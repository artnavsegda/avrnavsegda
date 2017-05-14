#include <asf.h>
#include "modesequence.h"
#include "main.h"
#include "i2c_api.h"
#include "pca9557_api.h"

extern int timetoexitmode;
extern int modenumber;
extern unsigned int runner[MEMORYUSE];
extern unsigned int temprunner[CALIBRATIONSECONDS];
extern int runflag;
extern int temprunflag;
extern int zerolevelavg;
extern int coefficent;
extern int celllevelavg;
extern int celltempavg;
extern struct settings mysettings;

int modeseconds(int modeneed)
{
	switch (modeneed)
	{
		case STARTLEVEL: return mysettings.length_table[0];
		case CELLDELAY: return mysettings.length_table[1];
		case CELLLEVEL:	return mysettings.length_table[2];
		case ZERODELAY: return mysettings.length_table[3];
		case ZEROTEST: return mysettings.length_table[4];
		case TOTALMERCURYDELAY: return mysettings.length_table[5];
		case TOTALMERCURY: return mysettings.length_table[6];
		case ELEMENTALMERCURYDELAY: return mysettings.length_table[7];
		case ELEMENTALMERCURY: return mysettings.length_table[8];
		case PRECALIBRATIONDELAY: return mysettings.length_table[9];
		case CALIBRATION: return mysettings.length_table[10];
		case POSTCALIBRATIONDELAY: return mysettings.length_table[11];
		case PURGE: return mysettings.length_table[12];
	}
	return 0;
}

 void entermode(int modetoenter)
{
	modenumber = modetoenter;
	timetoexitmode = modeseconds(modetoenter);
	writefloat(8,modetoenter);
	switch(modetoenter)
	{
		case STARTLEVEL:
			pca9557_set_pin_level(U3, U3_IGNIT, true);
		break;
		case CELLDELAY:
			pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, false);
			pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, true);
		break;
		case CELLLEVEL:
			pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, false);
			pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, true);
		break;
		case ZERODELAY:
			writecoil(REQUESTTOSTARTZEROTEST, 0);
			writecoil(STATUSOFCALIBRATION, 1);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		return;
		break;
		case ZEROTEST:
			writecoil(REQUESTTOSTARTZEROTEST, 0);
			writecoil(STATUSOFCALIBRATION, 1);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		break;
		case PURGE:
			writecoil(REQUESTTOSTARTPURGE, false);
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
			writecoil(REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY, false);
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case ELEMENTALMERCURY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case PRECALIBRATIONDELAY:
			writecoil(REQUESTTOSTARTCALIBRATION, false);
			writecoil(STATUSOFCALIBRATION, 1);
			//pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, false);
			//pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, true);
		break;
		case CALIBRATION:
			writecoil(REQUESTTOSTARTCALIBRATION, false);
			writecoil(STATUSOFCALIBRATION, 1);
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		case POSTCALIBRATIONDELAY:
			writecoil(REQUESTTOSTARTCALIBRATION, false);
			writecoil(STATUSOFCALIBRATION, 1);
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
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
			pca9557_set_pin_level(U3, U3_IGNIT, false);
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
			celllevelavg = average(runner,CELLLEVELSECONDS,runflag,MEMORYUSE)/CELLLEVELSECONDS;
			celltempavg = average(temprunner,CELLLEVELSECONDS,temprunflag,CELLLEVELSECONDS)/CELLLEVELSECONDS;
			pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, true);
			pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, false);
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
			zerolevelavg = average(runner,ZEROTESTSECONDS,runflag,MEMORYUSE)/ZEROTESTSECONDS;
			ioport_set_pin_level(LED0,true);
			pca9557_set_pin_level(U3, VALVE_ZM, false);
			writecoil(4, false);
		break;
		case PURGE:
			writecoil(103, false);
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
		break;
		case ELEMENTALMERCURY:
			ioport_set_pin_level(LED2,true);
			pca9557_set_pin_level(U3, VALVE_TE, false);
			writecoil(5, false);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
			coefficent = average(runner,CALIBRATIONSECONDS,runflag,MEMORYUSE)/CALIBRATIONSECONDS;
			ioport_set_pin_level(LED1,true);
			pca9557_set_pin_level(U3, VALVE_CM, false);
			//pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, true);
			//pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, false);
			//writecoil(5, false);
		break;
		case POSTCALIBRATIONDELAY:
			ioport_set_pin_level(LED1,false);
			pca9557_set_pin_level(U3, VALVE_CM, true);
			writecoil(5, false);
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit));
}

int sequence(int modetosequence)
{
	switch(modetosequence)
	{
		case STARTLEVEL:
			return CELLDELAY;
		break;
		case CELLDELAY:
			return CELLLEVEL;
		break;
		case CELLLEVEL:
			return TOTALMERCURYDELAY;
		break;
		case ZERODELAY:
			return ZEROTEST;
		break;
		case ZEROTEST:
			return TOTALMERCURYDELAY;
		break;
		case PURGE:
			return TOTALMERCURYDELAY;
		break;
		case TOTALMERCURYDELAY:
			return TOTALMERCURY;
		break;
		case TOTALMERCURY:
			return TOTALMERCURY;
		break;
		case ELEMENTALMERCURYDELAY:
			return ELEMENTALMERCURY;
		break;
		case ELEMENTALMERCURY:
			return TOTALMERCURY;
		break;
		case PRECALIBRATIONDELAY:
			return CALIBRATION;
		break;
		case CALIBRATION:
			return POSTCALIBRATIONDELAY;
		break;
		case POSTCALIBRATIONDELAY:
			return TOTALMERCURYDELAY;
		break;
		default:
			return modetosequence;
		break;
	}
	return modetosequence;
}
