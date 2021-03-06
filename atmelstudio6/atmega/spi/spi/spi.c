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

void startspi(void)
{
	DDRB = _BV(PORTB3)|_BV(PORTB5)|_BV(PORTB2);
	SPCR = _BV(SPE)|_BV(MSTR);
	SPSR |= _BV(SPI2X);
}

char spi_transfer(char c)
{
	SPDR = c;
	loop_until_bit_is_set(SPSR, SPIF);
	return(SPDR);
}

int main(void)
{
	startserial();
	startspi();
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0x20);
	spi_transfer(0x0C);
	spi_transfer(0x10);
	spi_transfer(0x40);
	while(1)
	{
		_delay_ms(100);
		spi_transfer(0x38);
		printf("%X%X\r\n",spi_transfer(0xFF),spi_transfer(0xFF));
	}
	return 0;
}
