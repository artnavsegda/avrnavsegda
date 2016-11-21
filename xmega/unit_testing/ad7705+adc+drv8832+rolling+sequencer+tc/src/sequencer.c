#include <asf.h>
#include "sequencer.h"
#include "rolling.h"

extern struct massive firststage, secondstage;

void tickmode(struct mydatastate *primarystate)
{
	primarystate->timetoexitmode--;
	if (primarystate->timetoexitmode == 0)
	exitmode(primarystate->currentmode,primarystate);
}

enum modelist sequence(enum modelist modetosequence)
{
	switch(modetosequence)
	{
		case STARTLEVEL: return ZERODELAY;
		case ZERODELAY:	return ZEROTEST;
		case ZEROTEST: return PRECALIBRATIONDELAY;
		case CELLDELAY: return CELLLEVEL;
		case CELLLEVEL:	return TOTALMERCURYDELAY;
		case TOTALMERCURYDELAY:	return TOTALMERCURY;
		case TOTALMERCURY: return TOTALMERCURY;
		default: return STARTLEVEL;
	}
	return modetosequence;
}

int modeseconds(enum modelist modeneed)
{
	switch (modeneed)
	{
		case STARTLEVEL: return 10;
		case ZERODELAY: return 10;
		case ZEROTEST: return 10;
		case CELLDELAY: return 10;
		case CELLLEVEL:	return 10;
		case TOTALMERCURYDELAY: return 10;
		case TOTALMERCURY: return 10;
		default: return 10;
	}
	return 0;
}

void entermode(enum modelist modetoenter, struct mydatastate *mystate)
{
	mystate->currentmode = modetoenter;
	mystate->timetoexitmode = modeseconds(modetoenter);
	switch(modetoenter)
	{
		case STARTLEVEL:
		break;
		case ZERODELAY:
			//printf("enabling green relay\n\r");
			pca9557_set_pin_low(mystate->x19_relay.address, mystate->x19_relay.pin_number);
			LED_On(LED3);
		break;
		case ZEROTEST:
		break;
		case CELLDELAY:
			drv8832_turn(mystate->settings.cell, DRV8832_LEFT);
		break;
		case CELLLEVEL:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		default:
		break;
	}
}

void exitmode(enum modelist modetoexit, struct mydatastate *mystate)
{
	switch(modetoexit)
	{
		case STARTLEVEL:
			pca9557_set_pin_dir(mystate->x19_relay.address, mystate->x19_relay.pin_number, PCA9557_DIR_OUTPUT);
			pca9557_set_pin_dir(mystate->x20_relay.address, mystate->x20_relay.pin_number, PCA9557_DIR_OUTPUT);
			//printf("\n\rHello ATMEL World!\n\r");
			pca9557_set_pin_high(mystate->x19_relay.address, mystate->x19_relay.pin_number);
			LED_Off(LED2);
			pca9557_set_pin_high(mystate->x20_relay.address, mystate->x20_relay.pin_number);
			LED_Off(LED3);
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
			mystate->zerolevelavg = oversample(&secondstage,modeseconds(ZEROTEST))/modeseconds(ZEROTEST);
			//printf("zerolevelavg is %u\n\r", mystate->zerolevelavg);
			pca9557_set_pin_high(mystate->x19_relay.address, mystate->x19_relay.pin_number);
			//printf("disabling green relay\n\r");
			LED_Off(LED3);
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
			mystate->celllevelavg = oversample(measurment_averaging_massive,modeseconds(CELLLEVEL));
			mystate->celltempavg = oversample(temperature_averaging_massive,modeseconds(CELLLEVEL));
			drv8832_turn(mystate->settings.cell, DRV8832_RIGHT);
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
			mystate->measurment = oversample(&secondstage,modeseconds(TOTALMERCURY))/modeseconds(TOTALMERCURY);
			//printf("measurment is %u\n\r", mystate->measurment);
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit), mystate);
}