#include <asf.h>
#include "setup.h"
#include "interrupt.h"

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 57600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void setup_init(void)
{
	pmic_init();
	sysclk_init();
	board_init();
	ioport_init();
	stdio_serial_init(&USARTC0, &usart_serial_options);
}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, tc_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_set_resolution(&TCC0, 31250);
}

void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	ioport_configure();
	tc_configure();
}

void setup_enable(void)
{
	cpu_irq_enable();
	tc_enable(&TCC0);
}