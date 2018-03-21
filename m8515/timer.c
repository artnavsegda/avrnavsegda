#include <avr/io.h>
#define F_CPU 2000000
#include <util/delay.h>

int main(void)
{
	DDRD |= _BV(PD5);
	TCCR1A |= _BV(WGM11)|_BV(COM1A1)|_BV(COM1A0);
	TCCR1B |= _BV(WGM13)|_BV(WGM12)|_BV(CS10);
	ICR1 = 19999;

	OCR1A = ICR1 - 2000;

	while(1)
	{
		OCR1A = ICR1 - 1400;
		_delay_ms(5000);
		OCR1A = ICR1 - 4800;
		_delay_ms(5000);
	}

}

