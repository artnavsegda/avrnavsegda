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
#include "i2c_api.h"

twi_master_options_t opt = {
	.speed = 50000,
};

void writefloat(uint8_t address, float content)
{
	i2c_send_word(&TWIE, 0x08, address, LSW(content));
	i2c_send_word(&TWIE, 0x08, address+1, MSW(content));
}

float readfloat(uint8_t address)
{
	float result = 0.0;
	LSW(result) = i2c_read_word(&TWIE,0x08,address);
	MSW(result) = i2c_read_word(&TWIE,0x08,address+1);
	return result;
}

int writecoil(uint8_t memory, uint8_t content)
{
	return i2c_send(&TWIE, 0x08, memory, content);
}

uint8_t readcoil(uint8_t memory)
{
	return i2c_read(&TWIE,0x08,memory);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	twi_master_setup(&TWIE, &opt);
	writecoil(0,true);//status of spectrometer
	writecoil(1,true);//status of thermocontrollers
	writecoil(2,true);//availability for external request
	writecoil(3,false);//status of zero test
	writecoil(4,false);//status of calibration
	writefloat(8,22.0);//code of current mode
	writefloat(10,1.0);//total
	writefloat(12,1.0);//oxidized
	writefloat(14,1.0);//monitor flow
	writefloat(16,1.0);//vacuum
	writefloat(18,1.0);//dilution pressure
	writefloat(20,1.0);//bypass pressure
	writefloat(22,1.0);//temp
	writefloat(24,1.0);//elemental
	writefloat(26,1.0);//not used
	writefloat(28,1.0);//errors and warnings
	writefloat(30,1.0);//coefficient
	writefloat(32,1.0);//lamp
	writecoil(99,false);//run calibration
	writecoil(100,false);//run zero test
	writecoil(101,false);//run elemental
	writecoil(102,false);//start purge
	writecoil(103,false);//end purge

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		delay_ms(100);
	}
}
