#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF;
	DDRE = 0xFF;
	PORTE = 0xFF;
	while (1)
	{
		PORTA = 0xFF;
		_delay_ms(1000);
		PORTA = 0x00;
		_delay_ms(1000);
	}
	return 0;
}

