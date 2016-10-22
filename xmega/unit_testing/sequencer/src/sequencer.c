#include <asf.h>
#include "sequencer.h"
#include "i2c.h"
#include "settings.h"

int modeseconds(enum modelist modeneed)
{
	struct mydatastruct mysettings;
	i2c_read_array(&TWIE,0x08,I2C_LENGTHTABLE,26,(uint8_t *)&mysettings.length_table);
	switch (modeneed)
	{
		case STARTLEVEL: return mysettings.length_table.startlevel;
		case CELLDELAY: return mysettings.length_table.celldelay;
		case CELLLEVEL:	return mysettings.length_table.celllevel;
		case ZERODELAY: return mysettings.length_table.zerodelay;
		case ZEROTEST: return mysettings.length_table.zerotest;
		case PURGE: return mysettings.length_table.purge;
		case TOTALMERCURYDELAY: return mysettings.length_table.totalmercurydelay;
		case TOTALMERCURY: return mysettings.length_table.totalmercury;
		case ELEMENTALMERCURYDELAY: return mysettings.length_table.elementalmercurydelay;
		case ELEMENTALMERCURY: return mysettings.length_table.elementalmercury;
		case PRECALIBRATIONDELAY: return mysettings.length_table.precalibrationdelay;
		case CALIBRATION: return mysettings.length_table.calibration;
		case POSTCALIBRATIONDELAY: return mysettings.length_table.postcalibrationdelay;
	}
	return 0;
}

enum modelist sequence(enum modelist modetosequence)
{
	struct mydatastruct mysettings;
	i2c_read_array(&TWIE,0x08,I2C_JUMPTABLE,26,(uint8_t *)&mysettings.jump_table);
	switch(modetosequence)
	{
		case STARTLEVEL: return mysettings.jump_table.startlevel;
		case CELLDELAY: return mysettings.jump_table.celldelay;
		case CELLLEVEL:	return mysettings.jump_table.celllevel;
		case ZERODELAY:	return mysettings.jump_table.zerodelay;
		case ZEROTEST: return mysettings.jump_table.zerotest;
		case PURGE:	return mysettings.jump_table.purge;
		case TOTALMERCURYDELAY:	return mysettings.jump_table.totalmercurydelay;
		case TOTALMERCURY: return mysettings.jump_table.totalmercury;
		case ELEMENTALMERCURYDELAY:	return mysettings.jump_table.elementalmercurydelay;
		case ELEMENTALMERCURY: return mysettings.jump_table.elementalmercury;
		case PRECALIBRATIONDELAY: return mysettings.jump_table.precalibrationdelay;
		case CALIBRATION: return mysettings.jump_table.calibration;
		case POSTCALIBRATIONDELAY: return mysettings.jump_table.postcalibrationdelay;
	}
	return modetosequence;
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
