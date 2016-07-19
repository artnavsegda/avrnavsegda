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

 void setupseconds(void)
 {
	 modeseconds[STARTLEVEL] = STARTLEVELSECONDS;
	 modeseconds[CELLDELAY] = CELLDELAYSECONDS;
	 modeseconds[CELLLEVEL] = CELLLEVELSECONDS;
	 modeseconds[ZERODELAY] = ZERODELAYSECONDS;
	 modeseconds[ZEROTEST] = ZEROTESTSECONDS;
	 modeseconds[PURGE] = PURGESECONDS;
	 modeseconds[TOTALMERCURYDELAY] = TOTALMERCURYDELAYSECONDS;
	 modeseconds[TOTALMERCURY] = TOTALMERCURYSECONDS;
	 modeseconds[ELEMENTALMERCURYDELAY] = ELEMENTALMERCURYDELAYSECONDS;
	 modeseconds[ELEMENTALMERCURY] = ELEMENTALMERCURYSECONDS;
	 modeseconds[PRECALIBRATIONDELAY] = PRECALIBRATIONDELAYSECONDS;
	 modeseconds[CALIBRATION] = CALIBRATIONSECONDS;
	 modeseconds[POSTCALIBRATIONDELAY] = POSTCALIBRATIONDELAYSECONDS;
 }

 void entermode(int modetoenter)
{
	modenumber = modetoenter;
	writefloat(8,modetoenter);
	switch(modetoenter)
	{
		case STARTLEVEL:
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
		break;
		case ZERODELAY:
			i2c_send(&TWIE, 0x08, 4, true);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		return;
		break;
		case ZEROTEST:
			i2c_send(&TWIE, 0x08, 4, true);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		break;
		case PURGE:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case ELEMENTALMERCURY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		case POSTCALIBRATIONDELAY:
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		default:
		break;
	}
	timetoexitmode = modeseconds[modetoenter];
	//delay_s(modeseconds[modetoenter]);
	//exitmode(modetoenter);
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
				ioport_set_pin_level(LED0,true);
				pca9557_set_pin_level(U3, VALVE_ZM, false);
				i2c_send(&TWIE, 0x08, 4, false);
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
				ioport_set_pin_level(LED2,true);
				pca9557_set_pin_level(U3, VALVE_TE, false);
				i2c_send(&TWIE, 0x08, 5, false);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
				ioport_set_pin_level(LED1,true);
				pca9557_set_pin_level(U3, VALVE_CM, false);
				i2c_send(&TWIE, 0x08, 5, false);
		break;
		case POSTCALIBRATIONDELAY:
				ioport_set_pin_level(LED1,true);
				pca9557_set_pin_level(U3, VALVE_CM, false);
				i2c_send(&TWIE, 0x08, 5, false);
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
			return TOTALMERCURY;
		break;
		case CELLDELAY:
			return CELLLEVEL;
		break;
		case CELLLEVEL:
			return TOTALMERCURY;
		break;
		case ZERODELAY:
			return ZEROTEST;
		break;
		case ZEROTEST:
			return TOTALMERCURY;
		break;
		case PURGE:
			return TOTALMERCURY;
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
			return TOTALMERCURY;
		break;
		default:
			return TOTALMERCURY;
		break;
	}
	return TOTALMERCURY;
}
