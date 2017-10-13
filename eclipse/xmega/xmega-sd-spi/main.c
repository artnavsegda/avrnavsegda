#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "timer.h"
#include "integer.h"
#include "diskio.h"
#include "mmc_avr.h"

volatile UINT Timer;    /* Performance timer (100Hz increment) */

ISR(TCC0_OVF_vect)
{
	Timer++;                        /* Performance counter for this module */
	disk_timerproc();       /* Drive timer procedure of low level disk I/O module */
}

int main()
{
	starttimer();
	startserial();
	printf("Hello world\r\n");
	// disable all spi devices for now, except sd card
	// PORTE.DIRSET = _BV(2); // sd card output
	// PORTE.OUTSET = _BV(2); // sd card cs level high
	PORTC.DIRSET = _BV(4); // ad7705 output
	PORTC.OUTSET = _BV(4); // ad7705 cs level high
	PORTC.DIRSET = _BV(0); // ethernet output
	PORTC.OUTSET = _BV(0); // ethernet cs level high
	// also disable wiznet SEN control
	PORTB.DIRSET = _BV(7); // wiznet SEN output
	// pretty unnecessary because output pin defaults to level low
	PORTB.OUTCLR = _BV(7); // wiznet SEN level low


	while(1)
	{
		printf("MMC status %d\r\n", mmc_disk_initialize());
		_delay_ms(1000);
	}
}
