#include <asf.h>
#include "setup.h"
#include "pca9557.h"
#include "i2c.h"

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void setup_init(void)
{
	board_init();
	sysclk_init();
	pmic_init();
	ioport_init();
}

void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void usart_configure(void)
{
	stdio_serial_init(&USARTC0, &usart_serial_options);
	usart_set_rx_interrupt_level(&USARTC0, USART_INT_LVL_LO);
}

void setup_configure(void)
{
	usart_configure();
	ioport_configure();
	twi_configure();
}

void setup_enable(void)
{
	cpu_irq_enable();
	twi_master_enable(&TWIE);
	pca9557_init(0x1a);
	i2c_send(&TWIE, 0x1a, 0x03, 0x00);
	i2c_send(&TWIE, 0x1a, 0x02, 0x00);
}