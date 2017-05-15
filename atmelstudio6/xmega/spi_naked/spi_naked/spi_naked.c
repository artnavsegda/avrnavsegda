#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(USARTC0.STATUS,USART_DREIF_bp);
	USARTC0.DATA = c;
	return 0;
}

void startserial(void)
{
	PORTC.DIRSET = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;

	USARTC0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false;
	USARTC0.BAUDCTRLA = 12;

	USARTC0.CTRLB |= USART_RXEN_bm;
	USARTC0.CTRLB |= USART_TXEN_bm;

	stdout = &mystdout;

	printf("serial started\n\r");
}

void startspi(void)
{
	PORTC.DIRSET = PIN7_bm;
	PORTC.DIRSET = PIN6_bm;
	//PORTC.DIRCLR = PIN5_bm;
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm;

	printf("spi started\n\r");
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	//loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	//while( !(SPIC.STATUS & SPI_IF_bm) );
	_delay_ms(1);
	return SPIC.DATA;
}

int main(void)
{
	startserial();
	startspi();
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
