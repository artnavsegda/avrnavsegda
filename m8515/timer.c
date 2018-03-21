#include <avr/io.h>
#define F_CPU 2000000
#include <util/delay.h>

#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
	PORTD |= _BV(PD4);
}

ISR(TIMER1_OVF_vect)
{
	PORTD &= ~_BV(PD4);
}

int main(void)
{
	DDRD |= _BV(PD5)|_BV(PD4);
	TCCR1A |= _BV(WGM11)|_BV(COM1A1)|_BV(COM1A0);
	TCCR1B |= _BV(WGM13)|_BV(WGM12)|_BV(CS10);

	TIMSK |= _BV(OCIE1A)|_BV(TOIE1);

	TIFR |= _BV(TOV1)|_BV(OCF1A);

	ICR1 = 19999;

	OCR1A = ICR1 - 2000;

	sei();

	while(1)
	{
		OCR1A = ICR1 - 1400;
		_delay_ms(5000);
		OCR1A = ICR1 - 4800;
		_delay_ms(5000);
	}

}

