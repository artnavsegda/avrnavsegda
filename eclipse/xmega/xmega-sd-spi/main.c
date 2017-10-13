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
	DWORD sz_drv, sz_eblk;
	WORD sz_sect;
	BYTE buff[FF_MAX_SS];
	DRESULT dr;

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
		printf("MMC status %d\r\n", disk_initialize(0));
		_delay_ms(1000);
		dr = disk_ioctl(0, GET_SECTOR_COUNT, &sz_drv);
        if (dr == RES_OK)
        	printf("Drive size %lu\r\n", sz_drv);
        else
            printf("Drive size ioctl failed.\n");
        _delay_ms(1000);
		dr = disk_ioctl(0, GET_SECTOR_SIZE, &sz_sect);
        if (dr == RES_OK)
        	printf("Sector size %u\r\n", sz_sect);
        else
            printf("Sector size ioctl failed.\n");
		_delay_ms(1000);
		dr = disk_ioctl(0, GET_BLOCK_SIZE, &sz_eblk);
        if (dr == RES_OK)
        	printf("Erase block is %lu sectors.\r\n", sz_eblk);
        else
            printf("Erase block sector size ioctl failed.\n");
		_delay_ms(1000);
		dr = disk_read(0, buff, 0, 1);
        if (dr == RES_OK)
        	printf("Disk read ok\r\n");
        else
            printf("Disk read failed.\n");
        _delay_ms(1000);
	}
}