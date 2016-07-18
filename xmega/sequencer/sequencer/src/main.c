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
#include "stdio.h"
#include "string.h"

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

char string[20];
int modenumber;

 int modeseconds[40];
 int timetoexitmode = 0;

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

void entermode(int modetoenter)
{
	modenumber = modetoenter;
	timetoexitmode = modeseconds[modetoenter];
}

void exitmode(int modetoexit)
{
	entermode(sequence(modetoexit));
}

static void sequence_callback(void)
{
	if (timetoexitmode-- <= 0)
		exitmode(modenumber);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	pmic_init();
	sysclk_init();
	setupseconds();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, sequence_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	entermode(STARTLEVEL);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV64_gc);

	/* Insert application code here, after the board has been initialized. */
	while (true)
	{
		snprintf(string, sizeof(string), "mode %d next mode %d", modenumber, sequence(modenumber));
		gfx_mono_draw_string(string,10,10,&sysfont);
		snprintf(string, sizeof(string), "length %d exit in %d next mode %d", modeseconds[modenumber], timetoexitmode, modeseconds[sequence(modenumber)]);
		gfx_mono_draw_string(string,20,20,&sysfont);
		delay_ms(1000);
	}
}
