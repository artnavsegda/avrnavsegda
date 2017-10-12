#include <avr/io.h>
#include <avr/interrupt.h>

void starttimer(void)
{
	OCR0A = F_CPU / 1024 / 100 - 1;
	TCCR0A = _BV(WGM01);
	TCCR0B = 0b101;
	TIMSK0 = _BV(OCIE0A);

	sei();
}
