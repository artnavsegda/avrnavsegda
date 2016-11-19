#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 57600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void setup_configure(void)
{
	twi_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIE);
	pca9557_init(0x18);
	pca9557_init(0x19);
	pca9557_init(0x1a);
}
