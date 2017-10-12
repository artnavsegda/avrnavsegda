#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "timer.h"
#include "integer.h"
#include "diskio.h"

volatile UINT Timer;    /* Performance timer (100Hz increment) */

ISR(TIMER0_COMPA_vect)
{
	Timer++;                        /* Performance counter for this module */
	disk_timerproc();       /* Drive timer procedure of low level disk I/O module */
}

int main()
{
	starttimer();
	startserial();
	printf("Hello world\r\n");

	while(1);
}
