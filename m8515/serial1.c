#include <avr/io.h>
#define F_CPU 2000000
#define BAUD 9600
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

unsigned short adc(unsigned char controlbyte)
{
	volatile unsigned char *p = (unsigned char *) 0x500;
	volatile unsigned short *r = 0x500;
	unsigned short z;
	*p = controlbyte;
	_delay_ms(1);
	loop_until_bit_is_clear(PORTD, PD2);
	z = *r;
	return z;
}

int main(void)
{
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;

#if USE_2X
	UCSRA |= _BV(U2X);
#else
	UCSRA &= ~(_BV(U2X));
#endif

	UCSRB = _BV(RXEN) | _BV(TXEN); /* Enable RX and TX */
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0); /* 8-bit data */

	MCUCR = 0x80; // XMEM enable
	//SFIOR = 0x78; //bus keeper enable
	SFIOR = 0x38; //bus keeper disable

	volatile unsigned char *p = (unsigned char *) 0x500;
	volatile unsigned char *d = (unsigned char *) 0x5FF;

	volatile unsigned short *r = 0x500;

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

	while (1)
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
	}
	return 0;
}
