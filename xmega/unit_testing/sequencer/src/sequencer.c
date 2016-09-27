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

void entermode(int modetoenter)
{
	currentmode = modetoenter;
	timetoexitmode = modeseconds(modetoenter);
}

int sequence(int modetosequence)
{
	return 10;
}

void exitmode(int modetoexit)
{
	entermode(sequence(modetoexit));
}
