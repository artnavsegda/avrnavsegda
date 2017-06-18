#include <asf.h>

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void AD7705_Init(void)
{
	ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_port_pin(&PORTC, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_port_pin(&PORTC, PIN6_bm, IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	spi_master_init(&SPIC);
	spi_enable(&SPIC);
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5);
	delay_ms(1);
	spi_write_packet(&SPIC, "\x20\x0C\x10\x04", 4);
	delay_ms(1);
	spi_write_packet(&SPIC, "\x60\x18\x3A\x00", 4);
	delay_ms(1);
	spi_write_packet(&SPIC, "\x70\x89\x78\xD7", 4);
	delay_ms(1);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
	AD7705_Init();
	printf("START\n\r");

	/* Insert application code here, after the board has been initialized. */
	while (1);
}
