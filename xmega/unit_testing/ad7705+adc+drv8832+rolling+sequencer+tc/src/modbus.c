#include <asf.h>
#include <stdio.h>
#include "modbus.h"
#include "sequencer.h"
#include "pca9557.h"

extern int16_t adc_scan_results[16];

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

void send_data(struct mydatastate *mystate)
{
	printf("\033[2Jtime to exit mode: %d\r",mystate->timetoexitmode);
	printf("current mode: %d\r",mystate->currentmode);
	printf("%03X %03X %03X %03X %03X\r", adc_scan_results[0], adc_scan_results[1], adc_scan_results[2], adc_scan_results[3], adc_scan_results[4]);
	printf("%03X %03X %03X %03X %03X\r", adc_scan_results[5], adc_scan_results[6], adc_scan_results[7], adc_scan_results[8], adc_scan_results[9]);
	printf("%03X %03X %03X %03X %03X\r", adc_scan_results[10], adc_scan_results[11], adc_scan_results[12], adc_scan_results[13], adc_scan_results[14]);
	printf("%03X\r", adc_scan_results[15]);

	int statusword = getstatus();
	printf("status of the spectrometer = %d\r", !(statusword & (LOW_LIGHT|LOW_FLOW)));
	printf("status of the THERMOCONTROLLERS = %d\r", !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4)));
	printf("AVAILABILITY OF EXTERNAL REQUEST = %d\r" , (mystate->currentmode == TOTALMERCURY));
	printf("STATUS OF ZEROTEST = %d\r", (mystate->currentmode == ZEROTEST || mystate->currentmode == ZERODELAY));
	printf("STATUS OF CALIBRATION = %d\r", (mystate->currentmode == CALIBRATION || mystate->currentmode == PRECALIBRATIONDELAY || mystate->currentmode == POSTCALIBRATIONDELAY));
	printf("MONITOR FLOW = %f\r", calculateflow(adc_voltage(adc_scan_results[2])));
	printf("VACUUM = %f\r", calculatepressure(adc_voltage(adc_scan_results[4])));
	printf("DILUTIONPRESSURE = %f\r", calculatepressure(adc_voltage(adc_scan_results[5])));
	printf("BYPASSPRESSURE = %f\r", calculatepressure(adc_voltage(adc_scan_results[6])));
	printf("TEMPERATUREOFSPECTROMETER = %f\r", (adc_voltage(adc_scan_results[3])-0.5)*100);
	printf("CODEOFACURRENTMODE = %d\r", mystate->currentmode);
	printf("ERRORSANDWARNINGS = %d\r", statusword);
}