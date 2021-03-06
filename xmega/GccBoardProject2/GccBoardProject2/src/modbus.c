#include <asf.h>
#include "sequencer.h"
#include "modbus.h"
#include "rolling.h"
#include "settings.h"
#include "i2c.h"
#include "pca9557.h"

extern uint16_t adc_scan_results[16];

struct massive ad7705_averaging_massive;
struct massive measurment_averaging_massive;
struct massive temperature_averaging_massive;

void process_data(struct mysettingsstruct *mysettings, struct mydatastate *mystate)
{
	if (mystate->currentmode == TOTALMERCURY||mystate->currentmode == PURGE)
	{
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTCALIBRATION)==1)
			entermode(PRECALIBRATIONDELAY,mystate);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTZEROTEST)==1)
			entermode(ZERODELAY,mystate);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY)==1)
			entermode(ELEMENTALMERCURYDELAY,mystate);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTPURGE)==1)
			entermode(PURGE,mystate);
		if (i2c_read(&TWIE, 0x08, REQUESTTOENDPURGE)==1)
			exitmode(PURGE,mystate);
	}
}

float adc_voltage(uint16_t adcvalue)
{
	const float popugai = (3.3/1.6)/4095;
	const int adczero = 180;
	return (adcvalue-adczero)*popugai;
}

int getstatus(void)
{
	int genstatus = 0;
	if (adc_voltage(adc_scan_results[0]) < 1.0) genstatus |= LOW_LIGHT;
	if (adc_voltage(adc_scan_results[2]) < 0.0) genstatus |= LOW_FLOW;
	if (pca9557_get_pin_level(U3,SERVO_4_RIGHT_IN))	genstatus |= CONVERTER;
	if (pca9557_get_pin_level(U2,SERVO_2_RIGHT_IN))	genstatus |= WATLOW1;
	if (pca9557_get_pin_level(U1,SERVO_2_LEFT_IN)) genstatus |= WATLOW2;
	if (pca9557_get_pin_level(U2,SERVO_3_RIGHT_IN))	genstatus |= WATLOW3;
	if (pca9557_get_pin_level(U2,SERVO_3_LEFT_IN)) genstatus |= WATLOW4;
	return genstatus;
}

float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration)
{
	return (
		(long) averaged - (long) zerolevelavg
	) / (float) (
		(long) coefficent - (long) zerolevelavg
	) * standard_concentration;
}

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor)
{
	return (
			(long) averaged - (long) zerolevelavg
	) / (float) (
		(long) celllevelavg - (long) zerolevelavg
	) * (
		c_twentie_five * exp (
			kfactor * (
				(
					(
						(
							(
								celltempavg - 180 // ADC zero level
							) * (
								(
									3.3 / 1.6 // Voltage reference
								) / 4095 // ADC resolution
							)
						) - 0.5
					) * 100.0 // temperature in Celsius
				) - 25.0
			)
		)
	);
}

#define FLOW_SENSOR_SPAN 10
#define EXPECTED_FLOW_SENSOR_VOLTAGE 9.0
#define RESISTOR_DIVIDER 0.319
#define R2_RESISTOROHM 3.3
#define R5_RESISTOROHM 10.0

float calculateflow(float voltage)
{
	return (
	(
		(
			voltage / (
				R2_RESISTOROHM/(
					R5_RESISTOROHM+R2_RESISTOROHM
				)
			)
		) / EXPECTED_FLOW_SENSOR_VOLTAGE
	) - 0.1
	)*(
		FLOW_SENSOR_SPAN / 0.4
	);
}

float calculatepressure(float voltage)
{
	return (voltage-0.4)*12;
}

void send_data(struct mysettingsstruct *mysettings, struct mydatastate *mystate)
{
	mysettings->standard_concentration = i2c_read_double(&TWIE, 0x08, I2C_STANDARDCONCENTRATION);
	mysettings->c_twentie_five = i2c_read_double(&TWIE, 0x08, I2C_C25);
	mysettings->kfactor = i2c_read_double(&TWIE, 0x08, I2C_KFACTOR);

	int statusword = getstatus();
	i2c_send(&TWIE, 0x08, I2C_STATUSOFSPECTROMETER, !(statusword & (LOW_LIGHT|LOW_FLOW)));
	i2c_send(&TWIE, 0x08, I2C_STATUSOFTHERMOCONTROLLERS, !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4)));
	i2c_send(&TWIE, 0x08, I2C_AVAILABILITYOFEXTERNALREQUEST, (mystate->currentmode == TOTALMERCURY));
	i2c_send(&TWIE, 0x08, I2C_STATUSOFZEROTEST, (mystate->currentmode == ZEROTEST || mystate->currentmode == ZERODELAY));
	i2c_send(&TWIE, 0x08, I2C_STATUSOFCALIBRATION, (mystate->currentmode == CALIBRATION || mystate->currentmode == PRECALIBRATIONDELAY || mystate->currentmode == POSTCALIBRATIONDELAY));
	if (mystate->currentmode == ELEMENTALMERCURY)	i2c_send_double(&TWIE, 0x08, I2C_ELEMENTALMERCURYROW, calculatecalibration(oversample(ad7705_averaging_massive, 32), mystate->zerolevelavg, mystate->coefficent, mysettings->standard_concentration));
	if (mystate->currentmode == TOTALMERCURY)	i2c_send_double(&TWIE, 0x08, I2C_TOTALMERCURYROW, calculatecell(oversample(ad7705_averaging_massive, 32), mystate->zerolevelavg, mystate->celllevelavg, mystate->celltempavg, mysettings->c_twentie_five, mysettings->kfactor));
	i2c_send_double(&TWIE, 0x08, I2C_MONITORFLOW, calculateflow(adc_voltage(adc_scan_results[2])));
	i2c_send_double(&TWIE, 0x08, I2C_VACUUM, calculatepressure(adc_voltage(adc_scan_results[4])));
	i2c_send_double(&TWIE, 0x08, I2C_DILUTIONPRESSURE, calculatepressure(adc_voltage(adc_scan_results[5])));
	i2c_send_double(&TWIE, 0x08, I2C_BYPASSPRESSURE, calculatepressure(adc_voltage(adc_scan_results[6])));
	i2c_send_double(&TWIE, 0x08, I2C_TEMPERATUREOFSPECTROMETER, (adc_voltage(adc_scan_results[3])-0.5)*100);
	i2c_send_double(&TWIE, 0x08, I2C_CODEOFACURRENTMODE, mystate->currentmode); // Code of a current mode
	i2c_send_double(&TWIE, 0x08, I2C_ERRORSANDWARNINGS, statusword); // Errors and warnings
	i2c_send_double(&TWIE, 0x08, I2C_TOTALMERCURYCOEFFICENT, mysettings->standard_concentration/(float)((long)mystate->coefficent-(long)mystate->zerolevelavg)); // Total mercury coefficent
}
