#include <asf.h>
#include "sequencer.h"

int timetoexitmode = 100;
int currentmode;

int modeseconds(int modeneed)
{
	switch (modeneed)
	{
		case STARTLEVEL: return STARTLEVELSECONDS;
		case CELLDELAY: return CELLDELAYSECONDS;
		case CELLLEVEL:	return CELLLEVELSECONDS;
		case ZERODELAY: return ZERODELAYSECONDS;
		case ZEROTEST: return ZEROTESTSECONDS;
		case PURGE: return PURGESECONDS;
		case TOTALMERCURYDELAY: return TOTALMERCURYDELAYSECONDS;
		case TOTALMERCURY: return TOTALMERCURYSECONDS;
		case ELEMENTALMERCURYDELAY: return ELEMENTALMERCURYDELAYSECONDS;
		case ELEMENTALMERCURY: return ELEMENTALMERCURYSECONDS;
		case PRECALIBRATIONDELAY: return PRECALIBRATIONDELAYSECONDS;
		case CALIBRATION: return CALIBRATIONSECONDS;
		case POSTCALIBRATIONDELAY: return POSTCALIBRATIONDELAYSECONDS;
	}
	return 0;
}

int sequence(int modetosequence)
{
	switch(modetosequence)
	{
		case STARTLEVEL: return CELLDELAY;
		case CELLDELAY: return CELLLEVEL;
		case CELLLEVEL:	return TOTALMERCURYDELAY;
		case ZERODELAY:	return ZEROTEST;
		case ZEROTEST: return TOTALMERCURYDELAY;
		case PURGE:	return TOTALMERCURYDELAY;
		case TOTALMERCURYDELAY:	return TOTALMERCURY;
		case TOTALMERCURY: return TOTALMERCURY;
		case ELEMENTALMERCURYDELAY:	return ELEMENTALMERCURY;
		case ELEMENTALMERCURY: return TOTALMERCURY;
		case PRECALIBRATIONDELAY: return CALIBRATION;
		case CALIBRATION: return POSTCALIBRATIONDELAY;
		case POSTCALIBRATIONDELAY: return TOTALMERCURYDELAY;
	}
	return modetosequence;
}

void entermode(int modetoenter)
{
	currentmode = modetoenter;
	timetoexitmode = modeseconds(modetoenter);
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

void exitmode(int modetoexit)
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
	entermode(sequence(modetoexit));
}
