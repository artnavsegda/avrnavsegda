#include <asf.h>

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

int main (void)
{
	uint16_t adcdata;
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
	spi_master_init(&SPIC);

	/* Insert application code here, after the board has been initialized. */
	printf("\n\rHello ATMEL World!\n\r");
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5);
	spi_write_packet(&SPIC, "\x20\x0C\x10\x40", 4);
	
	while (1)
	{
		delay_ms(100);
		spi_write_packet(&SPIC, "\x38", 1);
		spi_read_packet(&SPIC, (uint8_t *)&adcdata, 2);
		printf("%04X\r\n",adcdata);
	}
}
