#include <asf.h>
#include "sequencer.h"

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
		case PRECALIBRATIONDELAY: return CALIBRATION;
		case CALIBRATION: return POSTCALIBRATIONDELAY;
		case POSTCALIBRATIONDELAY: return TOTALMERCURYDELAY;
		case TOTALMERCURYDELAY:	return TOTALMERCURY;
		case TOTALMERCURY: return ZERODELAY;
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
		case PRECALIBRATIONDELAY: return 10;
		case CALIBRATION: return 10;
		case POSTCALIBRATIONDELAY: return 10;
		case TOTALMERCURYDELAY: return 10;
		case TOTALMERCURY: return 10;
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
			printf("\n\rHello ATMEL World!\n\r");
			pca9557_set_pin_high(x19_relay.address, x19_relay.pin_number);
			LED_Off(LED2);
			pca9557_set_pin_high(x20_relay.address, x20_relay.pin_number);
			LED_Off(LED3);
		break;
		case ZERODELAY:
			printf("enabling green relay\n\r");
			pca9557_set_pin_low(x19_relay.address, x19_relay.pin_number);
			LED_On(LED3);
		break;
		case ZEROTEST:
		break;
		case PRECALIBRATIONDELAY:
			printf("enabling red relay\n\r");
			pca9557_set_pin_low(x20_relay.address, x20_relay.pin_number);
			LED_On(LED2);
		break;
		case CALIBRATION:
		break;
		case POSTCALIBRATIONDELAY:
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
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
			zerolevelavg = oversample(&secondstage,10)/10;
			printf("zerolevelavg is %u\n\r", zerolevelavg);
			pca9557_set_pin_high(x19_relay.address, x19_relay.pin_number);
			printf("disabling green relay\n\r");
			LED_Off(LED3);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
			coefficent = oversample(&secondstage,10)/10;
			printf("coefficent is %u\n\r", coefficent);
			pca9557_set_pin_high(x20_relay.address, x20_relay.pin_number);
			printf("disabling red relay\n\r");
			LED_Off(LED2);
		break;
		case POSTCALIBRATIONDELAY:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		measurment = oversample(&secondstage,10)/10;
		printf("measurment is %u\n\r", measurment);
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit), mystate);
}