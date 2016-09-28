#include <asf.h>
#include "loop.h"
#include "sequencer.h"
#include "modbus.h"

extern uint16_t adc_scan_results[8];
extern uint16_t ad7705_raw_data, ad7705_averaged_data;
extern int currentmode;

void process_data(struct mydatastruct mydata)
{
	if (currentmode == TOTALMERCURY||currentmode == PURGE)
	{
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTCALIBRATION)==1)
			entermode(PRECALIBRATIONDELAY);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTZEROTEST)==1)
			entermode(ZERODELAY);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY)==1)
			entermode(ELEMENTALMERCURYDELAY);
		if (i2c_read(&TWIE, 0x08, REQUESTTOSTARTPURGE)==1)
			entermode(PURGE);
		if (i2c_read(&TWIE, 0x08, REQUESTTOENDPURGE)==1)
			exitmode(PURGE);
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
	if (adc_voltage(adc_scan_results[0]) < 1.0) genstatus|=LOW_LIGHT;
	if (adc_voltage(adc_scan_results[2]) < 0.0) genstatus|=LOW_FLOW;
	if (pca9557_get_pin_level(U3,SERVO_4_RIGHT_IN))	genstatus|=CONVERTER;
	if (pca9557_get_pin_level(U2,SERVO_2_RIGHT_IN))	genstatus|=WATLOW1;
	if (pca9557_get_pin_level(U1,SERVO_2_LEFT_IN)) genstatus|=WATLOW2;
	if (pca9557_get_pin_level(U2,SERVO_3_RIGHT_IN))	genstatus|=WATLOW3;
	if (pca9557_get_pin_level(U2,SERVO_3_LEFT_IN)) genstatus|=WATLOW4;
	return genstatus;
}

void send_data(struct mydatastruct mydata)
{
	/*i2c_send_word(&TWIE, 0x08, 0, adc_scan_results[0]);
	i2c_send_word(&TWIE, 0x08, 1, adc_scan_results[1]);
	i2c_send_word(&TWIE, 0x08, 2, adc_scan_results[2]);
	i2c_send_word(&TWIE, 0x08, 3, adc_scan_results[3]);
	i2c_send_word(&TWIE, 0x08, 4, adc_scan_results[4]);
	i2c_send_word(&TWIE, 0x08, 5, adc_scan_results[5]);
	i2c_send_word(&TWIE, 0x08, 6, adc_scan_results[6]);
	i2c_send_word(&TWIE, 0x08, 7, adc_scan_results[7]);
	i2c_send_word(&TWIE, 0x08, 8, ad7705_raw_data);
	i2c_send_word(&TWIE, 0x08, 9, ad7705_averaged_data);*/

	/*statusword = getstatus();
	writecoil(STATUSOFSPECTROMETER, !(statusword & (LOW_LIGHT|LOW_FLOW))); // Status of spectrometer
	writecoil(STATUSOFTHERMOCONTROLLERS, !(statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4))); // Status of thermo controllers
	writecoil(AVAILABILITYOFEXTERNALREQUEST, (modenumber == TOTALMERCURY)); // Availability of external request
	writecoil(STATUSOFZEROTEST, (modenumber == ZEROTEST || modenumber == ZERODELAY)); // Status of zero test
	writecoil(STATUSOFCALIBRATION, (modenumber == CALIBRATION || modenumber == PRECALIBRATIONDELAY || modenumber == POSTCALIBRATIONDELAY)); // Status of calibration
	if (modenumber == ELEMENTALMERCURY)	writefloat (24, calculatecalibration(mysettings.standard_concentration)); // elemental mercury
	if (modenumber == TOTALMERCURY)	writefloat (10, calculatecell(mysettings.c_twentie_five, mysettings.kfactor)); // total mercury
	writefloat(MONITORFLOW, calculateflow(analogVoltage(&ADCB, ADC_CH2))); // monitor flow
	writefloat(VACUUM, calculatepressure(analogVoltage(&ADCA, ADC_CH0))); // vacuum
	writefloat(DILUTIONPRESSURE, calculatepressure(analogVoltage(&ADCA, ADC_CH1))); // dilution pressure
	writefloat(BYPASSPRESSURE, calculatepressure(analogVoltage(&ADCA, ADC_CH2))); // bypass pressure
	writefloat(TEMPERATUREOFSPECTROMETER, (analogVoltage(&ADCB, ADC_CH3)-0.5)*100); // temperature of spectrometer
	//writefloat(26, analogRead(&ADCB, ADC_CH3)); // temperature ARB
	writefloat(CODEOFACURRENTMODE, modenumber); // Code of a current mode
	writefloat(ERRORSANDWARNINGS, statusword); // Errors and warnings
	writefloat(TOTALMERCURYCOEFFICENT, STANDARDCONCENTRATION/(float)((long)coefficent-(long)zerolevelavg)); // Total mercury coefficent*/
}

void display_data(struct mydatastruct mydata)
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
	struct mydatastruct mydata;
	while (true)
	{
		process_data(mydata);
		display_data(mydata);
		send_data(mydata);
		delay_ms(500);
	}
}