#include <asf.h>
#include "setup.h"

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void setup_init(void)
{
	pmic_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init();
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

void setup_configure(void)
{
	ioport_configure();
	usart_configure();
}

void setup_enable(void)
{
	udc_start();
}


void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void usart_configure(void)
{
	stdio_serial_init(&USARTC0, &usart_serial_options);
	usart_set_rx_interrupt_level(&USARTC0, USART_INT_LVL_LO);
}