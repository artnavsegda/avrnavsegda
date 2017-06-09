#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
{
	// toggle led here
	PORTD ^= _BV(PORTD5);//invert state
}

int main(void)
{
	DDRD = 0xFF;
	TCCR1B |= _BV(WGM12)|_BV(CS12)|_BV(CS10);
	TCNT1 = 0;
	OCR1A = 0x1E83;
	TIMSK |= _BV(OCIE1A);
	sei();
	while(1);
}

