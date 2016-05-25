/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "stdio.h"

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message);

char string[20];

twi_master_options_t opt = {
	.speed = 50000,
};

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message)
{
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = (void *)message, // transfer data source buffer
		.length       = sizeof(message)  // transfer data size (bytes)
	};
	return twi_master_write(twi, &packet);
}

ISR(PORTF_INT0_vect)
{
	i2c_send(&TWIE, 0x18, "\x01\x40");
}

ISR(PORTF_INT1_vect)
{
	i2c_send(&TWIE, 0x18, "\x01\x80");
}

int main (void)
{
	uint8_t data_received[1];
	twi_package_t packet_read = {
		.chip         = 0x18,      // TWI slave bus address
		.buffer       = data_received,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
	};
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	ioport_init();
	gfx_mono_init();
	twi_master_setup(&TWIE, &opt);

	/* Insert application code here, after the board has been initialized. */

	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_2,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_2, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	cpu_irq_enable();

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	//delay_ms(500);
	i2c_send(&TWIE, 0x18, "\x03\x3f"); // register 03, contents 3f
	//delay_ms(500);
	//i2c_send(&TWIE, 0x18, "\x01\x40"); // register 01, contents 40
	//delay_ms(500);
	//i2c_send(&TWIE, 0x18, "\x01\x80"); // register 01, contents 80
	//delay_ms(500);*/

	while (true) {
		if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
		twi_master_read(&TWIE, &packet_read);
//			//Check read content
			//data_received[0];
			snprintf(string,sizeof(string),"hello %3d",data_received[0]);
			gfx_mono_draw_string(string,10,10,&sysfont);
			delay_ms(500);
		}
	}
}
