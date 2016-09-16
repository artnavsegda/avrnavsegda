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
#include "i2c_api.h"
#include "adc_api.h"

char string[20];

twi_master_options_t opt = {
	.speed = 50000,
};

ISR(PORTF_INT0_vect)
{
	i2c_send(&TWIE, 0x1a, 0x01, 0x00);
}

ISR(PORTF_INT1_vect)
{
	i2c_send(&TWIE, 0x1a, 0x01, 0xff);
}

int main (void)
{
	struct eeprom
	{
		uint8_t ip[4];
		uint8_t mac[6];
		uint16_t length_table[13];
		uint8_t jump_table[13];
	};
	struct eeprom e;
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	ioport_init();
	gfx_mono_init();
	twi_master_setup(&TWIE, &opt);
	adc_init();

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
	irq_initialize_vectors();
	cpu_irq_enable();

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	//i2c_send_word(&TWIE, 0x08, 0x03, 0x1234); // register 03, contents 3f

	while (true) {
		i2c_read_array(&TWIE, 0x8, 100, 4, e.ip);
		snprintf(string,sizeof(string),"IP: %hu.%hu.%hu.%hu",e.ip[0],e.ip[1],e.ip[2],e.ip[3]);
		gfx_mono_draw_string(string,10,10,&sysfont);
		i2c_read_array(&TWIE, 0x8, 101, 6, e.mac);
		snprintf(string,sizeof(string),"mac: %hx:%hx:%hx:%hx:%hx:%hx",e.mac[0],e.mac[1],e.mac[2],e.mac[3],e.mac[4],e.mac[5]);
		gfx_mono_draw_string(string,10,10,&sysfont);
		i2c_send_word(&TWIE, 0x08, 0, analogVoltage(&ADCB, ADC_CH0));
		i2c_send_word(&TWIE, 0x08, 1, analogVoltage(&ADCB, ADC_CH1));
		i2c_send_word(&TWIE, 0x08, 2, analogVoltage(&ADCB, ADC_CH2));
		i2c_send_word(&TWIE, 0x08, 3, analogVoltage(&ADCB, ADC_CH3));
		i2c_send_word(&TWIE, 0x08, 4, analogVoltage(&ADCA, ADC_CH0));
		i2c_send_word(&TWIE, 0x08, 5, analogVoltage(&ADCA, ADC_CH1));
		i2c_send_word(&TWIE, 0x08, 6, analogVoltage(&ADCA, ADC_CH2));
		i2c_send_word(&TWIE, 0x08, 7, analogVoltage(&ADCA, ADC_CH3));
		i2c_send_double(&TWIE, 0x08, 100, analogVoltage(&ADCB, ADC_CH0));
		i2c_send_double(&TWIE, 0x08, 101, analogVoltage(&ADCB, ADC_CH1));
		i2c_send_double(&TWIE, 0x08, 102, analogVoltage(&ADCB, ADC_CH2));
		i2c_send_double(&TWIE, 0x08, 103, analogVoltage(&ADCB, ADC_CH3));
		i2c_send_double(&TWIE, 0x08, 104, analogVoltage(&ADCA, ADC_CH0));
		i2c_send_double(&TWIE, 0x08, 105, analogVoltage(&ADCA, ADC_CH1));
		i2c_send_double(&TWIE, 0x08, 106, analogVoltage(&ADCA, ADC_CH2));
		i2c_send_double(&TWIE, 0x08, 107, analogVoltage(&ADCA, ADC_CH3));
		delay_ms(500);
	}
}
