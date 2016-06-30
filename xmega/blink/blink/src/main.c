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

#define PCA9557_IO0 0
#define PCA9557_IO1 1
#define PCA9557_IO2 2
#define PCA9557_IO3 3
#define PCA9557_IO4 4
#define PCA9557_IO5 5
#define PCA9557_IO6 6
#define PCA9557_IO7 7

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

twi_master_options_t opt = {
	.speed = 50000,
};

ISR(PORTF_INT0_vect)
{
	ioport_toggle_pin(LCD_BACKLIGHT_ENABLE_PIN);
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

void pca9557_init(uint8_t addr)
{
	// polarity all bits retained
	i2c_send(&TWIE, addr, 0x02, 0x00);
}

uint8_t pca9557_get_pin_level(uint8_t addr, uint8_t port)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x00);
	return state & _BV(port);
}

void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x01);
	if (level)
		state |= _BV(port);
	else
		state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x01, state);
};

void pca9557_toggle_pin(uint8_t addr, uint8_t port)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x01);
	if (state & _BV(port))
		state &= ~_BV(port);
	else
		state |= _BV(port);
	i2c_send(&TWIE, addr, 0x01, state);
}

void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x03);
	if (dir == PCA9557_DIR_INPUT)
		state = state | port;
	else if (dir == PCA9557_DIR_OUTPUT)
		state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x03, state);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	ioport_init();
	board_init();
	twi_master_setup(&TWIE, &opt);
	//i2c_send(&TWIE, 0x1a, 0x02, 0x00);
	pca9557_init(0x1a);

	/* Insert application code here, after the board has been initialized. */

	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 0, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO1, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO2, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO3, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO4, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO5, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO6, IOPORT_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, PCA9557_IO7, IOPORT_DIR_OUTPUT);
	//i2c_send(&TWIE, 0x1a, 0x03, 0x00);
	ioport_set_pin_dir(LCD_BACKLIGHT_ENABLE_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc;
	irq_initialize_vectors()
	cpu_irq_enable();

	//bool value;

	while (1)
	{
		//value = ioport_get_pin_level(GPIO_PUSH_BUTTON_1);
		//ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN,value);
		//ioport_toggle_pin(LED0);
		//ioport_toggle_pin(LED1);
		//ioport_toggle_pin(LED2);
		//ioport_toggle_pin(LED3);
		pca9557_toggle_pin(0x1a, 0);
		pca9557_toggle_pin(0x1a, PCA9557_IO1);
		pca9557_toggle_pin(0x1a, PCA9557_IO2);
		pca9557_toggle_pin(0x1a, PCA9557_IO3);
		pca9557_toggle_pin(0x1a, PCA9557_IO4);
		pca9557_toggle_pin(0x1a, PCA9557_IO5);
		pca9557_toggle_pin(0x1a, PCA9557_IO6);
		pca9557_toggle_pin(0x1a, PCA9557_IO7);
		//i2c_send(&TWIE, 0x1a, 0x01, 0x00);
		//delay_ms(500);
		//pca9557_set_pin_level(0x1a, 0, false);
		//delay_ms(500);
		//i2c_send(&TWIE, 0x1a, 0x01, 0xff);
		//pca9557_set_pin_level(0x1a, 0, true);
		delay_ms(500);
	}
}
