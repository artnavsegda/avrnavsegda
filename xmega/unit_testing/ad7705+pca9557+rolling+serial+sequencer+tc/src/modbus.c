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

void send_data(struct mydatastate *mystate)
{
	printf("\033[2Jtime to exit mode: %d\r",mystate->timetoexitmode);
	printf("current mode: %d\r",mystate->currentmode);

	if (mystate->currentmode == TOTALMERCURY)
		printf("calculated value is %f\r", (float)(adcdata/64-mystate->zerolevelavg)/(mystate->coefficent-mystate->zerolevelavg));

	printf("%6lX", oversample(&firststage,16)/16);
	printf("%6lX", oversample(&firststage,32)/32);
	printf("%6lX", oversample(&firststage,64)/64);
	printf("%6lX\r", oversample(&secondstage,8)/8);
}