#include <asf.h>
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
	gfx_mono_init();
}

void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	ioport_configure();
}

void setup_enable(void)
{
	cpu_irq_enable();
}