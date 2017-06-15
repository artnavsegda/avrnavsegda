#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUD 9600
#include <util/setbaud.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

void startserial(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
	stdout = &mystdout;
	printf("Hello eclipse World!\n\r");
}

void starttimer(void)
{
	TCCR2A = 0x33;
	TCCR2B = 0x09;
	OCR2A = 0x06;
	OCR2B = 0x03;
}

char spi_transfer(char c)
{
	SPDR = c;
	loop_until_bit_is_set(SPSR, SPIF);
	return(SPDR);
}

uint32_t shiftIn(void)
{
	uint32_t value = 0;
	uint8_t i;
	for (i = 0;i < 24; ++i)
	{
		PORTD |= _BV(PORTD4);
		if (bit_is_set(PIND,PIND5))
			value |= (long)1<<(24-i);
		PORTD &= ~(_BV(PORTD4));
	}
	PORTD |= _BV(PORTD4);
	PORTD &= ~(_BV(PORTD4));// 25th pulse to keep DATA high till next data ready
	return value;
}

int main(void)
{
	DDRD = _BV(PORTD1)|_BV(PORTD3)|_BV(PORTD4);//tx,clk,sck
	startserial();
	starttimer();

    while(1)
    {
			if (bit_is_clear(PIND,PIND5))
			{
				printf("%lX\r\n",shiftIn());
			}
    }
	return 0;
}
