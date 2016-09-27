#include <asf.h>
#include "loop.h"

extern uint16_t adc_scan_results[8];
extern uint16_t ad7705_raw_data, ad7705_averaged_data;

void recieve_data(struct mydatastruct mysettings)
{
	i2c_read_array(&TWIE,0x08,I2C_IPADDRESS,4,mysettings.ip);
	i2c_read_array(&TWIE,0x08,I2C_MACADDRESS,6,mysettings.mac);
}

void process_data(struct mydatastruct mydata)
{
	/*if (modenumber == TOTALMERCURY||modenumber == PURGE)
	{
		if (readcoil(REQUESTTOSTARTCALIBRATION)==1) // Request to start calibration
		entermode(PRECALIBRATIONDELAY);
		if (readcoil(REQUESTTOSTARTZEROTEST)==1) // Request to start zero test
		entermode(ZERODELAY);
		if (readcoil(REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY)==1) // Request to start measurement of elemental mercury
		entermode(ELEMENTALMERCURYDELAY);
		if (readcoil(REQUESTTOSTARTPURGE)==1) // Request to start purge
		entermode(PURGE);
		if (readcoil(REQUESTTOENDPURGE)==1) // Request to end purge
		exitmode(PURGE);
	}*/
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
		recieve_data(mydata);
		process_data(mydata);
		display_data(mydata);
		send_data(mydata);
		delay_ms(500);
	}
}