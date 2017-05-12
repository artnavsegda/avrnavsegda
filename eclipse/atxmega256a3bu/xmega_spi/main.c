#include <avr/io.h>
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

	printf("Hello eclipse World!\n\r");
}

void startspi(void)
{
	SPIC.CTRL = 0xC0;    // SPI slave, clock idle low, data setup on trailing edge, data sampled on leading edge, double speed mode enabled
	SPIC.INTCTRL = 0x00; // ensure SPI interrupts are disabled
}

char spi_transfer(char c)
{
	SPIC.DATA = c;
	loop_until_bit_is_set(SPIC.STATUS,SPI_IF_bp);
	return SPIC.DATA;
}

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code
    }
}
