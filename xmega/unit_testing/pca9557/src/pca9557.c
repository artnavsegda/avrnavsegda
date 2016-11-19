#include <asf.h>
#include "i2c.h"
#include "pca9557.h"

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
	configmask[addr-0x18] = state; // write output back to own memory
	i2c_send(&TWIE, addr, 0x03, state);
	printf("direction byte of %X is %X\n\r",addr,state);
}

void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level)
{
	uint8_t state;
	//state = i2c_read(&TWIE,addr,0x01);
	state = outputmask[addr-0x18]; // ignore actual register values and use own memory instead
	if (level)
		state &= ~_BV(port);
	else
		state |= _BV(port);
	//i2c_send(&TWIE, addr, 0x03, configmask[addr-0x18]); // set direction specifically
	outputmask[addr-0x18] = state; // write output back to own memory
	i2c_send(&TWIE, addr, 0x01, state);
	printf("output byte of %X is %X\n\r",addr,state);
};

bool pca9557_get_pin_level(uint8_t addr, uint8_t port)
{
	uint8_t state;
	// polarity all bits retained
	i2c_send(&TWIE, addr, 0x02, 0x00);
	state = i2c_read(&TWIE,addr,0x00);
	return state & _BV(port);
}
