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

char string[20];

twi_master_options_t opt = {
	.speed = 50000,
};

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content) // ??????? i2c ??????
{
	status_code_t status;
	uint8_t message[2];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 2  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = content;
	status = twi_master_write(twi, &packet);
	return status;
}

status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content) // ??????? i2c ??????
{
  status_code_t status;
  uint8_t message[3];
  twi_package_t packet = {
    .chip         = addr,      // TWI slave bus address
    .buffer       = message, // transfer data source buffer
    .length       = 3  // transfer data size (bytes)
  };
  message[0] = memory;
  message[1] = MSB(content);
  message[2] = LSB(content);
  status = twi_master_write(twi, &packet);
  return status;
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory)
{
   status_code_t status;
   uint8_t message[1];
   twi_package_t packet = {
     .chip         = addr,      // TWI slave bus address
     .buffer       = message,        // transfer data destination buffer
     .length       = 1                    // transfer data size (bytes)
   };
   message[0] = memory;
   status = twi_master_write(twi, &packet);
   if(status == TWI_SUCCESS)
   {
     status_code_t status = twi_master_read(twi, &packet);
     if(status == TWI_SUCCESS)
		return message[0];
   }
   return status;
}
 
uint16_t i2c_read_word(TWI_t *twi, uint8_t addr, uint8_t memory)
{
   status_code_t status;
   uint16_t recievedword;
   uint8_t message[2];
   twi_package_t packet = {
     .chip         = addr,      // TWI slave bus address
     .buffer       = message,        // transfer data destination buffer
     .length       = 1                    // transfer data size (bytes)
   };
   message[0] = memory;
   status = twi_master_write(twi, &packet);
   if(status == TWI_SUCCESS)
   {
     packet.length = 2;
     status_code_t status = twi_master_read(twi, &packet);
     if(status == TWI_SUCCESS)
     {
		MSB(recievedword) = message[0];
		LSB(recievedword) = message[1];
		return recievedword;
     }    
	}
	return status;
}

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
	irq_initialize_vectors();
	cpu_irq_enable();

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	//i2c_send_word(&TWIE, 0x08, 0x03, 0x1234); // register 03, contents 3f

	while (true) {
		snprintf(string,sizeof(string),"%3x",i2c_read(&TWIE, 0x8, 100));
		gfx_mono_draw_string(string,10,10,&sysfont);
		delay_ms(500);
	}
}
