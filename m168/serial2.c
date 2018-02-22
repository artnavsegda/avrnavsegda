#include <avr/io.h>
#define F_CPU 1000000
#define BAUD 9600
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void write(unsigned char address, unsigned char data)
{
        DDRA = 0xFF; // AD line as output
        PORTE |= _BV(PE1); // open address latch
        PORTA = address; // set address
        _delay_us(1);
        PORTE &= ~_BV(PE1); // close address latch
        PORTA = data;
        PORTD &= ~_BV(PD6); // WR line strobe low start
        _delay_us(1);
        PORTD |= _BV(PD6); // WR line strobe low end
}

unsigned char read(unsigned char address)
{
        unsigned char x;
        DDRA = 0xFF; // AD line as output
        PORTE |= _BV(PE1); // open address latch
        PORTA = address; // set address
        _delay_us(1);
        PORTE &= ~_BV(PE1); // close address latch

        DDRA = 0x00; // tristate AD line for data read
        PORTD &= ~_BV(PD7); // RD line strobe low start
        _delay_us(1);
        x = PINA;
        PORTD |= _BV(PD7); // RD line strobe low end
        return x;
}

unsigned short adc(unsigned char controlbyte)
{
	//volatile unsigned char *p = (unsigned char *) 0x500;
	//volatile unsigned short *r = 0x500;
	unsigned short z;
	write(0x00,controlbyte);
	//*p = controlbyte;
	_delay_ms(1);
	loop_until_bit_is_clear(PORTD, PD2);
	((char *)&z)[0] = read(0x00);
	((char *)&z)[1] = read(0x01);
	//z = *r;
	return z;
}

int main(void)
{
	DDRE |= _BV(PE1); // ALE line as otput
	DDRD |= _BV(PD6)|_BV(PD7); // WR/RD output
	PORTD |= _BV(PD6)|_BV(PD7); // WR/RD idle high

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
	UCSR0C = _BV(URSEL0) | _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */

	//MCUCR = 0x80; // XMEM enable
	//SFIOR = 0x78; //bus keeper enable
	//SFIOR = 0x38; //bus keeper disable

	//volatile unsigned char *p = (unsigned char *) 0x500;
	//volatile unsigned char *d = (unsigned char *) 0x5FF;

	//volatile unsigned short *r = 0x500;

	unsigned char x;
	unsigned short z;

	stdout = &mystdout;
	_delay_ms(500);
	//printf("hello\r\n");

	/*while (1)
	{
		//printf("hello");
		uart_putchar('a',NULL);
		_delay_ms(1000);
	}*/	

	/*while (1)
	{
		x = *p;
		_delay_ms(1000);
		x = *d;
		_delay_ms(1000);
	}*/

	while (1)
	{
		printf("%x %x %x %x %x %x %x %x\r\n",adc(0), adc(1), adc(2), adc(3), adc(4), adc(5), adc(6), adc(7));
	}

	/*while (1)
	{
		*p = 0x00;
		_delay_ms(100);
		z = *r;
		printf("%x ch0\r\n",z);
		_delay_ms(1000);
		*p = 0x01;
		_delay_ms(100);
		z = *r;
		printf("%x ch1\r\n",z);
		_delay_ms(1000);
	}

	while (1)
	{
		*p = 0x00;
		_delay_ms(1000);
		x = *p;
		//printf("%x read\r\n",x);
		_delay_ms(1000);	
		//*d = 0x00;
		//_delay_ms(1000);
	}

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
	}*/
	return 0;
}

