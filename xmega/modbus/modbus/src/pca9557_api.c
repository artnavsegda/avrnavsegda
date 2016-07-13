/*
 * pca9557_api.c
 *
 * Created: 30-Jun-16 8:06:29
 *  Author: artna
 */ 

 #include <asf.h>
 #include "pca9557_api.h"
 #include "i2c_api.h"

 void pca9557_init(uint8_t addr)
 {
	 // polarity all bits retained
	 i2c_send(&TWIE, addr, 0x02, 0x00);
 }

 void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir)
 {
	 uint8_t state;
	 state = i2c_read(&TWIE,addr,0x03);
	 if (dir == PCA9557_DIR_INPUT)
		state |= _BV(port);
	 else if (dir == PCA9557_DIR_OUTPUT)
		state &= ~_BV(port);
	 i2c_send(&TWIE, addr, 0x03, state);
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

bool pca9557_get_pin_level(uint8_t addr, uint8_t port)
 {
	 uint8_t state;
	 i2c_send(&TWIE, addr, 0x02, 0x00);
	 state = i2c_read(&TWIE,addr,0x00);
	 return state & _BV(port);
 }
