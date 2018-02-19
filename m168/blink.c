#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF;
	MCUCR = 0x80;
	SFIOR = 0x78; //bus keeper enable
	//SFIOR = 0x38; //bus keeper disable

	volatile unsigned char *p = (unsigned char *) 0x500;
	volatile unsigned char *d = (unsigned char *) 0x501;

	volatile unsigned short *r = 0x500;

	unsigned char x;
	unsigned int z;

	while (1)
	{
		*p = 0x00;
		_delay_ms(100);

		z = *r;

		_delay_ms(100);
		PORTC = ~((char *)&z)[0];
		_delay_ms(100);
		PORTC = ~((char *)&z)[1];

		/* x = *p;
		_delay_ms(100);
		PORTC = ~x;
		_delay_ms(100);	
		x = *d;
		_delay_ms(100);
		PORTC = ~x;
		_delay_ms(100);	*/
	}
	return 0;
}

