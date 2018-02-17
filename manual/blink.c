#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	MCUCR = 0x80;
	//SFIOR = 0x78; //bus keeper enable
	SFIOR = 0x38; //bus keeper disable

	unsigned char * p = 0x500;
	unsigned char * d = 0x5FF;
	unsigned char x;

	while (1)
	{
		*p = 0xFF;
		_delay_ms(1000);
		*d = 0xFF;
		_delay_ms(1000);

		*p = 0x00;
		_delay_ms(1000);
		*d = 0x00;
		_delay_ms(1000);

		*p = *d;
		_delay_ms(1000);
		*d = !*p;
		_delay_ms(1000);
	}
	return 0;
}

