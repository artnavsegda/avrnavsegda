#include <asf.h>
#include <stdio.h>
#include <math.h>
#include "modbus.h"
#include "sequencer.h"
#include "pca9557.h"
#include "rolling.h"

extern int16_t adc_scan_results[16];
extern struct massive firststage, secondstage;
extern uint16_t adcdata;

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor)
{
	return (float)(
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

void process_data(struct mysettingsstruct *mysettings, struct mydatastate *mystate)
{
	if (mystate->currentmode == TOTALMERCURY)
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

void send_data(struct mydatastate *mystate)
{
	printf("\033[2Jtime to exit mode: %d\r",mystate->timetoexitmode);
	printf("current mode: %d\r",mystate->currentmode);

	if (mystate->currentmode == TOTALMERCURY)
		printf("calculated value is %f\r", calculatecell(adcdata,mystate->zerolevelavg,mystate->celllevelavg,mystate->celltempavg,mystate->c_twentie_five,mystate->kfactor));

	printf("%6lX", oversample(&firststage,16)/16);
	printf("%6lX", oversample(&firststage,32)/32);
	printf("%6lX", oversample(&firststage,64)/64);
	printf("%6lX\r", oversample(&secondstage,8)/8);
}