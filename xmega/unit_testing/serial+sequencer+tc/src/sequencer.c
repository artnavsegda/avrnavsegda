#include <asf.h>
#include "sequencer.h"

void tickmode(struct mydatastate *primarystate)
{
	primarystate->timetoexitmode--;
	if (primarystate->timetoexitmode == 0)
		exitmode(primarystate->currentmode,primarystate);
}

void exitmode(enum modelist modetoexit, struct mydatastate *mystate)
{
	switch(modetoexit)
	{
		case STARTLEVEL:
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
		break;
		case PURGE:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
		break;
		case ELEMENTALMERCURY:
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
		break;
		case POSTCALIBRATIONDELAY:
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit), mystate);
}

enum modelist sequence(enum modelist modetosequence)
{
	switch(modetosequence)
	{
		case STARTLEVEL: return CELLDELAY;
		case CELLDELAY: return CELLLEVEL;
		case CELLLEVEL:	return ZERODELAY;
		case ZERODELAY:	return ZEROTEST;
		case ZEROTEST: return PURGE;
		case PURGE:	return TOTALMERCURYDELAY;
		case TOTALMERCURYDELAY:	return TOTALMERCURY;
		case TOTALMERCURY: return ELEMENTALMERCURYDELAY;
		case ELEMENTALMERCURYDELAY:	return ELEMENTALMERCURY;
		case ELEMENTALMERCURY: return PRECALIBRATIONDELAY;
		case PRECALIBRATIONDELAY: return CALIBRATION;
		case CALIBRATION: return POSTCALIBRATIONDELAY;
		case POSTCALIBRATIONDELAY: return STARTLEVEL;
	}
	return modetosequence;
}

int modeseconds(enum modelist modeneed)
{
	switch (modeneed)
	{
		case STARTLEVEL: return 10;
		case CELLDELAY: return 10;
		case CELLLEVEL:	return 10;
		case ZERODELAY: return 10;
		case ZEROTEST: return 10;
		case PURGE: return 10;
		case TOTALMERCURYDELAY: return 10;
		case TOTALMERCURY: return 10;
		case ELEMENTALMERCURYDELAY: return 10;
		case ELEMENTALMERCURY: return 10;
		case PRECALIBRATIONDELAY: return 10;
		case CALIBRATION: return 10;
		case POSTCALIBRATIONDELAY: return 10;
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
		case CELLDELAY:
		break;
		case CELLLEVEL:
		break;
		case ZERODELAY:
		return;
		break;
		case ZEROTEST:
		break;
		case PURGE:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
		break;
		case ELEMENTALMERCURY:
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
		break;
		case POSTCALIBRATIONDELAY:
		break;
		default:
		break;
	}
}