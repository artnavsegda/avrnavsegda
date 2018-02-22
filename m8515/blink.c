#define F_CPU 2000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF; // AD line as output
	DDRC = 0xFF;
	DDRE |= _BV(PE1); // ALE line as otput
	DDRD |= _BV(PD6)|_BV(PD7); // WR/RD output
	PORTD |= _BV(PD6)|_BV(PD7); // WR/RD idle high
	// MCUCR = 0x80; // XMEM enable
	// SFIOR = 0x78; //bus keeper enable
	// SFIOR = 0x38; //bus keeper disable

//	volatile unsigned char *p = (unsigned char *) 0x500;
//	volatile unsigned char *d = (unsigned char *) 0x501;

//	volatile unsigned short *r = 0x500;

	unsigned char x;
	unsigned int z;

	while (1)
	{
		DDRA = 0xFF; // AD line as output

		PORTE |= _BV(PE1); // open address latch
		PORTA = 0x00; // set address to zero
		PORTE &= ~_BV(PE1); // close address latch

		PORTA = 0x00; // set data to zero

		PORTD &= ~_BV(PD6); // WR line strobe low start
		PORTD |= _BV(PD6); // WR line strobe low end
		
		//*p = 0x00; same but in hardware
		_delay_ms(100);


		DDRA = 0x00; // tristate AD line for data read

		PORTD &= ~_BV(PD7); // RD line strobe low start
		x = PORTA;
		PORTD |= _BV(PD7); // RD line strobe low end
		
		//x = *p; same but in hardware
		_delay_ms(100);


		/*z = *r;
		_delay_ms(100);

		PORTC = ~((char *)&z)[0];
		_delay_ms(100);
		PORTC = ~((char *)&z)[1];*/

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

