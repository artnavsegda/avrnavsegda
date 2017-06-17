/*
 * main.c
 *
 *  Created on: Jun 8, 2017
 *      Author: artnavsegda
 */


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0xFF;
	while (1)
	{
		PORTD ^= _BV(PORTD5);//invert state
		_delay_ms(1000);
	}
	return 0;
}

