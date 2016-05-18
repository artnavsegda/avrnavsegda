#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	PORTA.DIR = 0xFF;
	while (1)
	{
		PORTA.OUT = 0xFF;
		_delay_ms(1000);
		PORTA.OUT = 0x00;
		_delay_ms(1000);
	}
	return 0;
}

