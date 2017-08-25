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
	
	ioport_set_pin_dir(SPIC_SS,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPIC_MOSI,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPIC_MISO,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SPIC_SCK,IOPORT_DIR_OUTPUT);
	
	stdio_serial_init(&USARTC0, &usart_serial_options);
	spi_master_init(&SPIC);
	spi_enable(&SPIC);

	/* Insert application code here, after the board has been initialized. */
	printf("\n\rMCU started\n\r");
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5);
	delay_ms(10);
	printf("Reset complete\n\r");
	spi_write_packet(&SPIC, "\x20\x0C", 2);
	delay_ms(10);
	spi_write_packet(&SPIC, "\x10\x04", 2);
	delay_ms(10);
	spi_write_packet(&SPIC, "\x60\x18\x3A\x00", 4);
	delay_ms(10);
	spi_write_packet(&SPIC, "\x70\x89\x78\xD7", 4);
	delay_ms(10);
	
	while (1)
	{
		if (ioport_get_pin_level(J1_PIN1) == false)
		{
			spi_write_packet(&SPIC, "\x38", 1);
			spi_read_packet(&SPIC, (uint8_t *)&adcdata, 2);
			printf("%04X\r\n",adcdata);
		}
	}
}
