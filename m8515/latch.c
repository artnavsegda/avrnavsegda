#include <avr/io.h>
#define F_CPU 2000000
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF; // PORTA output
	DDRB |= _BV(PB3); // latch strobe input
	DDRC |= _BV(PC7);

	_delay_ms(1000);

	while(1)
	{
		PORTA |= _BV(PA2);
		_delay_ms(100);
		PORTB &= ~_BV(PB3); // drop front low
		_delay_ms(100);
		PORTB |= _BV(PB3); // raise front high
		
		_delay_ms(1000);

		PORTA &= ~_BV(PA2);
		_delay_ms(100);
		PORTB &= ~_BV(PB3); // drop front low
		_delay_ms(100);
		PORTB |= _BV(PB3); // raise front high

		_delay_ms(1000);
	}
}
