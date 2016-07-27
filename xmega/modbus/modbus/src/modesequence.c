/*
 * modesequence.c
 *
 * Created: 14-Jul-16 11:27:43
 *  Author: Art Navsegda
 */ 

 #include <asf.h>
 #include "modesequence.h"
 #include "main.h"
 #include "i2c_api.h"
 #include "pca9557_api.h"

 //void setupseconds(void)
 //{
	 //modeseconds[STARTLEVEL] = STARTLEVELSECONDS;
	 //modeseconds[CELLDELAY] = CELLDELAYSECONDS;
	 //modeseconds[CELLLEVEL] = CELLLEVELSECONDS;
	 //modeseconds[ZERODELAY] = ZERODELAYSECONDS;
	 //modeseconds[ZEROTEST] = ZEROTESTSECONDS;
	 //modeseconds[PURGE] = PURGESECONDS;
	 //modeseconds[TOTALMERCURYDELAY] = TOTALMERCURYDELAYSECONDS;
	 //modeseconds[TOTALMERCURY] = TOTALMERCURYSECONDS;
	 //modeseconds[ELEMENTALMERCURYDELAY] = ELEMENTALMERCURYDELAYSECONDS;
	 //modeseconds[ELEMENTALMERCURY] = ELEMENTALMERCURYSECONDS;
	 //modeseconds[PRECALIBRATIONDELAY] = PRECALIBRATIONDELAYSECONDS;
	 //modeseconds[CALIBRATION] = CALIBRATIONSECONDS;
	 //modeseconds[POSTCALIBRATIONDELAY] = POSTCALIBRATIONDELAYSECONDS;
 //}

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

 void entermode(int modetoenter)
{
	modenumber = modetoenter;
	timetoexitmode = modeseconds(modetoenter);
	writefloat(8,modetoenter);
	switch(modetoenter)
	{
		case STARTLEVEL:
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
			writecoil(100, 0);
			writecoil(4, 1);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		return;
		break;
		case ZEROTEST:
			writecoil(100, 0);
			writecoil(4, 1);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		break;
		case PURGE:
			writecoil(102, false);
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
			writecoil(101, false);
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case ELEMENTALMERCURY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case PRECALIBRATIONDELAY:
			writecoil(99, false);
			writecoil(4, 1);
			//pca9557_set_pin_level(U1, SERVO_1_LEFT_OUT, false);
			//pca9557_set_pin_level(U1, SERVO_1_RIGHT_OUT, true);
		break;
		case CALIBRATION:
			writecoil(99, false);
			writecoil(4, 1);
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		case POSTCALIBRATIONDELAY:
			writecoil(99, false);
			writecoil(4, 1);
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
