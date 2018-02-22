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

void write(unsigned char data)
{
        DDRA = 0xFF; // AD line as output

        PORTA = data;
        PORTD &= ~_BV(PD6); // WR line strobe low start
        _delay_us(1);
        PORTD |= _BV(PD6); // WR line strobe low end
}

unsigned char read(void)
{
        unsigned char x;

        DDRA = 0x00; // tristate AD line for data read
        PORTD &= ~_BV(PD7); // RD line strobe low start
        _delay_us(1);
        x = PINA;
        PORTD |= _BV(PD7); // RD line strobe low end
        return x;
}

unsigned short adc(unsigned char controlbyte)
{
	unsigned short z;
	write(controlbyte);
	_delay_ms(1);
	loop_until_bit_is_clear(PORTD, PD2);
	PORTB &= ~_BV(PD5);
	((char *)&z)[0] = read();
	PORTB |= _BV(PD5);		
	((char *)&z)[1] = read();
	return z;
}

int main(void)
{
	DDRB |= _BV(PB3)|_BV(PB4)|_BV(PB5); // gpio latch cs, mac cs, max hben as outputs
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

	unsigned char x;
	unsigned short z;

	stdout = &mystdout;
	_delay_ms(500);
	//printf("hello\r\n");

	while (1)
	{
		printf("%x %x %x %x %x %x %x %x\r\n",adc(0x40), adc(0x41), adc(0x42), adc(0x43), adc(0x44), adc(0x45), adc(0x46), adc(0x47));
	}

	return 0;
}

