#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm;
	PORTC.DIRSET = PIN5_bm;
	PORTC.DIRSET = PIN4_bm;//always activate chip select output when enabling spi
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;

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
	while(1)
	{
		if (bit_is_clear(PORTC.IN,PIN1_bp))
		{
			spi_transfer(0x38);
			spi_array((char *)&result,2);
			printf("%04X\r\n",__builtin_bswap16(result));
		}
	}
	return 0;
}
