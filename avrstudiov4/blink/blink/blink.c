#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	PORTD_DIRSET = _BV(4);
	while(1)
	{
		PORTD_OUTTGL = _BV(4);
		_delay_ms(1000);
	}
}
