#include <asf.h>
#include "i2c.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
}

void twi_configure(void)
{
	twi_master_options_t opt = {
		.speed = 50000,
		.chip  = 0x50
	};
	twi_master_setup(&TWIC, &opt);
}

void sequence_callback(void)
{

}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, sequence_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
}

void setup_configure(void)
{
	twi_configure();
	tc_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIC);
	tc_enable(&TCC0);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
}

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

uint8_t polaritymask[3];
uint8_t outputmask[3];
uint8_t configmask[3];

void pca9557_init(uint8_t addr)
{
	// output register all bits level low
	outputmask[addr-0x18] = 0x00;
	i2c_send(&TWIE, addr, 0x01, 0x00);
	// polarity all bits retained
	polaritymask[addr-0x18] = 0x00;
	i2c_send(&TWIE, addr, 0x02, 0x00);
	// setting all bits ports input
	configmask[addr-0x18] = 0xFF;
	i2c_send(&TWIE, addr, 0x03, 0xFF);
}

void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir)
{
	uint8_t state;
	//state = i2c_read(&TWIE,addr,0x03);
	state = configmask[addr-0x18]; // ignore actual register values and use own memory instead
	if (dir == PCA9557_DIR_INPUT)
	state |= _BV(port);
	else if (dir == PCA9557_DIR_OUTPUT)
	state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x03, state);
}

void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level)
{
	uint8_t state;
	//state = i2c_read(&TWIE,addr,0x01);
	state = outputmask[addr-0x18]; // ignore actual register values and use own memory instead
	if (level)
	state |= _BV(port);
	else
	state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x03, configmask[addr-0x18]); // set direction specifically
	i2c_send(&TWIE, addr, 0x01, state);
};

bool pca9557_get_pin_level(uint8_t addr, uint8_t port)
{
	uint8_t state;
	i2c_send(&TWIE, addr, 0x02, 0x00);
	state = i2c_read(&TWIE,addr,0x00);
	return state & _BV(port);
}
