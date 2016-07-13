/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "i2c_api.h"
#include "pca9557_api.h"

#define STARTLEVEL 5
#define CELLDELAY 7
#define CELLLEVEL 8
#define ZERODELAY 11
#define ZEROTEST 12
#define PURGE 13
#define TOTALMERCURYDELAY 21
#define TOTALMERCURY 22
#define ELEMENTALMERCURYDELAY 26
#define ELEMENTALMERCURY 27
#define PRECALIBRATIONDELAY 31
#define CALIBRATION 32
#define POSTCALIBRATIONDELAY 33

#define STARTLEVELSECONDS 18
#define CELLDELAYSECONDS 120
#define CELLLEVELSECONDS 5
#define ZERODELAYSECONDS 12
#define ZEROTESTSECONDS 30
#define PURGESECONDS 32768
#define TOTALMERCURYDELAYSECONDS 15
#define TOTALMERCURYSECONDS 720
#define ELEMENTALMERCURYDELAYSECONDS 5
#define ELEMENTALMERCURYSECONDS 100
#define PRECALIBRATIONDELAYSECONDS 360
#define CALIBRATIONSECONDS 600
#define POSTCALIBRATIONDELAYSECONDS 900

//u1
#define U1 0x18
#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define RELAY_1 0
//u2
#define U2 0x19
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define RELAY_2 0
//u3
#define U3 0x1a
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6
#define VALVE_ZM 2 //x10:3
#define VALVE_CM 3 //x10:4
#define VALVE_TE 4 //x10:5
#define VALVE_RE 5 //x10:6
#define U3_IGNIT 1

void entermode(int modetoenter);
void exitmode(int modetoexit);
int sequence(int modetosequence);
void setupseconds(void);
void writefloat(uint8_t address, float content);
float readfloat(uint8_t address);
int writecoil(uint8_t memory, uint8_t content);
uint8_t readcoil(uint8_t memory);

twi_master_options_t opt = {
	.speed = 50000,
};

int modeseconds[40];

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

void writefloat(uint8_t address, float content)
{
	i2c_send_word(&TWIE, 0x08, address, LSW(content));
	i2c_send_word(&TWIE, 0x08, address+1, MSW(content));
}

float readfloat(uint8_t address)
{
	float result = 0.0;
	LSW(result) = i2c_read_word(&TWIE,0x08,address);
	MSW(result) = i2c_read_word(&TWIE,0x08,address+1);
	return result;
}

int writecoil(uint8_t memory, uint8_t content)
{
	return i2c_send(&TWIE, 0x08, memory, content);
}

uint8_t readcoil(uint8_t memory)
{
	return i2c_read(&TWIE,0x08,memory);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	twi_master_setup(&TWIE, &opt);
	writecoil(0,true);//status of spectrometer
	writecoil(1,true);//status of thermocontrollers
	writecoil(2,true);//availability for external request
	writecoil(3,false);//status of zero test
	writecoil(4,false);//status of calibration
	writefloat(8,22.0);//code of current mode
	writefloat(10,1.0);//total
	writefloat(12,1.0);//oxidized
	writefloat(14,1.0);//monitor flow
	writefloat(16,1.0);//vacuum
	writefloat(18,1.0);//dilution pressure
	writefloat(20,1.0);//bypass pressure
	writefloat(22,1.0);//temp
	writefloat(24,1.0);//elemental
	writefloat(26,1.0);//not used
	writefloat(28,1.0);//errors and warnings
	writefloat(30,1.0);//coefficient
	writefloat(32,1.0);//lamp
	writecoil(99,false);//run calibration
	writecoil(100,false);//run zero test
	writecoil(101,false);//run elemental
	writecoil(102,false);//start purge
	writecoil(103,false);//end purge

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		delay_ms(100);
		if (readcoil(99))
		{
			writecoil(99,false);
			entermode(CALIBRATION);
		}
		if (readcoil(100))
		{
			writecoil(100,false);
			entermode(ZERODELAY);
		}
		if (readcoil(101))
		{
			writecoil(101,false);
			entermode(ELEMENTALMERCURY);
		}
		if (readcoil(102))
		{
			writecoil(102,false);
			entermode(PURGE);
		}
		if (readcoil(103))
		{
			writecoil(103,false);
			entermode(TOTALMERCURY);
		}
	}
}

void entermode(int modetoenter)
{
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
	delay_s(modeseconds[modetoenter]);
	exitmode(modetoenter);
}

void logic_init(void)
{
	pca9557_init(0x18);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, RELAY_1, PCA9557_DIR_OUTPUT);
	pca9557_init(0x19);
	pca9557_set_pin_dir(0x19, SERVO_2_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, RELAY_2, PCA9557_DIR_OUTPUT);
	pca9557_init(0x1a);
	pca9557_set_pin_dir(0x1a, SERVO_4_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, SERVO_4_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, VALVE_ZM, PCA9557_DIR_OUTPUT); //x10:3
	pca9557_set_pin_dir(0x1a, VALVE_CM, PCA9557_DIR_OUTPUT); //x10:4
	pca9557_set_pin_dir(0x1a, VALVE_TE, PCA9557_DIR_OUTPUT); //x10:5
	pca9557_set_pin_dir(0x1a, VALVE_RE, PCA9557_DIR_OUTPUT); //x10:6
	pca9557_set_pin_dir(0x1a, U3_IGNIT, PCA9557_DIR_OUTPUT);
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