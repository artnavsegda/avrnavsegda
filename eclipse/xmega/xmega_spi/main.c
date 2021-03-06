#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm | PIN0_bm; //always activate chip select output when enabling spi
	PORTB.DIRSET = PIN7_bm; // SEN wiznet
	//PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
	PORTE.DIRSET = PIN2_bm;
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;
	PORTC.OUTSET = PIN0_bm | PIN4_bm;
	PORTE.OUTSET = PIN2_bm;
	//PORTB.OUTCLR = PIN7_bm; // disable wiznet spi

	printf("spi started\n\r");
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	return SPIC.DATA;
}

void spi_array(char *buffer, unsigned NoBytes)
{
        int i;
        for (i = 0; i < NoBytes; i++)
        {
				SPIC.DATA = buffer[i];
				loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
				buffer[i] = SPIC.DATA;
        }
}

int main(void)
{
	unsigned int result;
	_delay_ms(1000);
	startserial();
	startspi();
	PORTC.OUTCLR = PIN4_bm;
	spi_array("\xFF\xFF\xFF\xFF\xFF", 5);
	_delay_ms(10);
	spi_array("\x20\x0C", 2);
	_delay_ms(10);
	spi_array("\x10\x04", 2);
	_delay_ms(10);
	spi_array("\x60\x18\x3A\x00", 4);
	_delay_ms(10);
	spi_array("\x70\x89\x78\xD7", 4);
	_delay_ms(10);
	PORTC.OUTSET = PIN4_bm;
	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			PORTC.OUTCLR = PIN4_bm;
			spi_transfer(0x38);
			spi_array((char *)&result,2);
			printf("%04X\r\n",__builtin_bswap16(result));
			PORTC.OUTSET = PIN4_bm;
		}
	}
	return 0;
}
