#include <asf.h>
#include "sequencer.h"
#include "i2c.h"
#include "settings.h"

extern struct massive measurment_averaging_massive;
extern struct massive temperature_averaging_massive;

void decrement_mode_counter(struct mydatastate mystate)
{
	mystate.timetoexitmode--;
	
	if (mystate.timetoexitmode == 0)
		exitmode(mystate.currentmode, mystate);
}

int modeseconds(int modeneed)
{
	struct mydatastruct mysettings;
	i2c_read_array(&TWIE,0x08,I2C_LENGTHTABLE,26,mysettings.length_table);
	switch (modeneed)
	{
		case STARTLEVEL: return mysettings.length_table[0];
		case CELLDELAY: return mysettings.length_table[1];
		case CELLLEVEL:	return mysettings.length_table[2];
		case ZERODELAY: return mysettings.length_table[3];
		case ZEROTEST: return mysettings.length_table[4];
		case PURGE: return mysettings.length_table[5];
		case TOTALMERCURYDELAY: return mysettings.length_table[6];
		case TOTALMERCURY: return mysettings.length_table[7];
		case ELEMENTALMERCURYDELAY: return mysettings.length_table[8];
		case ELEMENTALMERCURY: return mysettings.length_table[9];
		case PRECALIBRATIONDELAY: return mysettings.length_table[10];
		case CALIBRATION: return mysettings.length_table[11];
		case POSTCALIBRATIONDELAY: return mysettings.length_table[12];
	}
	return 0;
}

int sequence(int modetosequence)
{
	struct mydatastruct mysettings;
	i2c_read_array(&TWIE,0x08,I2C_JUMPTABLE,13,mysettings.jump_table);
	switch(modetosequence)
	{
		case STARTLEVEL: return mysettings.jump_table[0];
		case CELLDELAY: return mysettings.jump_table[1];
		case CELLLEVEL:	return mysettings.jump_table[2];
		case ZERODELAY:	return mysettings.jump_table[3];
		case ZEROTEST: return mysettings.jump_table[4];
		case PURGE:	return mysettings.jump_table[5];
		case TOTALMERCURYDELAY:	return mysettings.jump_table[6];
		case TOTALMERCURY: return mysettings.jump_table[7];
		case ELEMENTALMERCURYDELAY:	return mysettings.jump_table[8];
		case ELEMENTALMERCURY: return mysettings.jump_table[9];
		case PRECALIBRATIONDELAY: return mysettings.jump_table[10];
		case CALIBRATION: return mysettings.jump_table[11];
		case POSTCALIBRATIONDELAY: return mysettings.jump_table[12];
	}
	return modetosequence;
}

void entermode(int modetoenter, struct mydatastate mystate)
{
	mystate.currentmode = modetoenter;
	mystate.timetoexitmode = modeseconds(modetoenter);
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

void exitmode(int modetoexit, struct mydatastate mystate)
{
	switch(modetoexit)
	{
		case STARTLEVEL:
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
			mystate.celllevelavg = oversample(measurment_averaging_massive,modeseconds(CELLLEVEL));
			mystate.celltempavg = oversample(temperature_averaging_massive,modeseconds(CELLLEVEL));
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
			mystate.zerolevelavg = oversample(measurment_averaging_massive,modeseconds(ZEROTEST));
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
			mystate.coefficent = oversample(measurment_averaging_massive,modeseconds(CALIBRATION));
		break;
		case POSTCALIBRATIONDELAY:
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit), mystate);
}
