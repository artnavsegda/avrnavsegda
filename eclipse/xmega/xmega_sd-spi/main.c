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
#include "ff.h"

volatile UINT Timer;    /* Performance timer (100Hz increment) */

ISR(TCC0_OVF_vect)
{
	Timer++;                        /* Performance counter for this module */
	disk_timerproc();       /* Drive timer procedure of low level disk I/O module */
}

int main()
{
	FATFS fs;
	DIR dir;
	FILINFO fi;

	starttimer();
	startserial();
	printf("Hello world\r\n");

	/* Configure MOSI/MISO/SCLK/CS pins (PD5-4-3 = H-L-H) */
	PORTC.DIRSET = (_BV(5)|_BV(7)|_BV(4)|_BV(0)); // MOSI,SCLK,CS output
	PORTB.DIRSET = _BV(7); // wiznet SEN output
	PORTE.DIRSET = _BV(2); // sd card output
	PORTC.OUTSET = _BV(7); // SCLK level high
	// disable all spi devices for now, except sd card
	PORTE.OUTSET = _BV(2); // sd card cs level high
	PORTC.OUTSET = _BV(4); // ad7705 cs level high
	PORTC.OUTSET = _BV(0); // ethernet cs level high
	PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc; // MISO level pull up
	// also disable wiznet SEN control
	// pretty unnecessary because output pin defaults to level low
	PORTB.OUTCLR = _BV(7); // wiznet SEN level low

	/* Enable SPI module in SPI mode 0 */
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;

	f_mount(&fs, "", 0);
	f_opendir(&dir, "/");
	while(1)
	{
		f_readdir(&dir,&fi);
		if (fi.fname[0] == 0)
			break;
		printf("%s\r\n", fi.fname);
	}

	while(1);
}
