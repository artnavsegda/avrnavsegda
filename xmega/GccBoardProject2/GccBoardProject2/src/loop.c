#include <asf.h>
#include "loop.h"
#include "sequencer.h"
#include "modbus.h"

extern uint16_t adc_scan_results[8];
extern uint16_t ad7705_raw_data, ad7705_averaged_data;

void process_data(struct mydatastruct mysettings, struct mydatastate mystate)
{
	if (mystate.currentmode == TOTALMERCURY||mystate.currentmode == PURGE)
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

void send_data(struct mydatastruct mysettings, struct mydatastate mystate)
{
	i2c_read_double(&TWIE, 0x08, I2C_STANDARDCONCENTRATION, mysettings.standard_concentration);
	i2c_read_double(&TWIE, 0x08, I2C_C25, mysettings.c_twentie_five);
	i2c_read_double(&TWIE, 0x08, I2C_KFACTOR, mysettings.kfactor);

	int statusword = getstatus();
	i2c_send(&TWIE, 0x08, STATUSOFSPECTROMETER, !(statusword & (LOW_LIGHT|LOW_FLOW))); // Status of spectrometer
	i2c_send(&TWIE, 0x08, STATUSOFTHERMOCONTROLLERS, !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4))); // Status of thermo controllers
	i2c_send(&TWIE, 0x08, AVAILABILITYOFEXTERNALREQUEST, (mystate.currentmode == TOTALMERCURY)); // Availability of external request
	i2c_send(&TWIE, 0x08, STATUSOFZEROTEST, (mystate.currentmode == ZEROTEST || mystate.currentmode == ZERODELAY)); // Status of zero test
	i2c_send(&TWIE, 0x08, STATUSOFCALIBRATION, (mystate.currentmode == CALIBRATION || mystate.currentmode == PRECALIBRATIONDELAY || mystate.currentmode == POSTCALIBRATIONDELAY)); // Status of calibration
	if (mystate.currentmode == ELEMENTALMERCURY)	i2c_send_double(&TWIE, 0x08, ELEMENTALMERCURYROW, calculatecalibration(oversample(ad7705_averaging_massive, 32), mystate.zerolevelavg, mystate.coefficent, mysettings.standard_concentration)); // elemental mercury
	if (mystate.currentmode == TOTALMERCURY)	i2c_send_double(&TWIE, 0x08, TOTALMERCURYROW, calculatecell(oversample(ad7705_averaging_massive, 32), mystate.zerolevelavg, mystate.celllevelavg, mystate.celllevelavg, mysettings.c_twentie_five, mysettings.kfactor)); // total mercury
	i2c_send_double(&TWIE, 0x08, MONITORFLOW, calculateflow(adc_voltage(adc_scan_results[2]))); // monitor flow
	i2c_send_double(&TWIE, 0x08, VACUUM, calculatepressure(adc_voltage(adc_scan_results[4]))); // vacuum
	i2c_send_double(&TWIE, 0x08, DILUTIONPRESSURE, calculatepressure(adc_voltage(adc_scan_results[5]))); // dilution pressure
	i2c_send_double(&TWIE, 0x08, BYPASSPRESSURE, calculatepressure(adc_voltage(adc_scan_results[6]))); // bypass pressure
	i2c_send_double(&TWIE, 0x08, TEMPERATUREOFSPECTROMETER, (adc_voltage(adc_scan_results[3])-0.5)*100); // temperature of spectrometer
	i2c_send_double(&TWIE, 0x08, CODEOFACURRENTMODE, mystate.currentmode); // Code of a current mode
	i2c_send_double(&TWIE, 0x08, ERRORSANDWARNINGS, statusword); // Errors and warnings
	i2c_send_double(&TWIE, 0x08, TOTALMERCURYCOEFFICENT, mysettings.standard_concentration/(float)((long)mystate.coefficent-(long)mystate.zerolevelavg)); // Total mercury coefficent
}

void display_data(struct mydatastruct mydata, struct mydatastate mystate)
{
	/*switch (displaymode)
	{
		case 1:
			display_mode_one();
		break;
		default:
		break;
	}*/
}

void loop(void)
{
	//struct mydatastruct mydata;
	while (true)
	{
		//process_data(mydata);
		//display_data(mydata);
		//send_data(mydata);
		delay_ms(500);
	}
}

void sequence_callback(void)
{
	static struct mydatastate primarystate;
	static struct mydatastruct mydata;

	decrement_mode_counter(primarystate);
	increment(measurment_averaging_massive, oversample(ad7705_averaging_massive, 32));
	increment(temperature_averaging_massive, adc_scan_results[3]);
	process_data(mydata,primarystate);
	display_data(mydata,primarystate);
	send_data(mydata,primarystate);
}