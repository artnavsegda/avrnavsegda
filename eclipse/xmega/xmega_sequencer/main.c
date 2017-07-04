#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"
#include "spi.h"
#include "i2c.h"

void AD7705_Init(void)
{
	startspi();
	PORTC.OUTCLR = PIN4_bm;
	spi_array("\xFF\xFF\xFF\xFF\xFF", 5);
	_delay_ms(1);
	spi_array("\x20\x0C", 2);
	_delay_ms(1);
	spi_array("\x10\x04", 2);
	_delay_ms(1);
	spi_array("\x60\x18\x3A\x00", 4);
	_delay_ms(1);
	spi_array("\x70\x89\x78\xD7", 4);
	_delay_ms(1);
}

int main(void)
{
	unsigned int result;
	startserial();
	startspi();
	AD7705_Init();
	starti2c();
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
