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
	
    ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
    ioport_configure_port_pin(&PORTC, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
    //ioport_configure_port_pin(&PORTC, PIN6_bm, IOPORT_DIR_INPUT);
    ioport_configure_port_pin(&PORTC, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	stdio_serial_init(&USARTC0, &usart_serial_options);
	spi_master_init(&SPIC);
	spi_enable(&SPIC);

	/* Insert application code here, after the board has been initialized. */
	printf("\n\rMCU started\n\r");
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5);
	printf("Reset complete\n\r");
	spi_write_packet(&SPIC, "\x20\x0C\x10\x40", 4);
	
	while (1)
	{
		delay_ms(100);
		spi_write_packet(&SPIC, "\x38", 1);
		spi_read_packet(&SPIC, (uint8_t *)&adcdata, 2);
		printf("%04X\r\n",adcdata);
	}
}
