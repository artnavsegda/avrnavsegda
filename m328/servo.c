#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

int main(void)
{
	DDRB |= _BV(PB1);
	TCCR1A |= _BV(WGM11)|_BV(COM1A1)|_BV(COM1A0);
	TCCR1B |= _BV(WGM13)|_BV(WGM12)|_BV(CS11);
	ICR1 = 19999;

	OCR1A = ICR1 - 2000;

	while(1)
	{
		OCR1A = ICR1 - 800;
		_delay_ms(1000);
		OCR1A = ICR1 - 2200;
		_delay_ms(1000);
	}

}
