#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

enum drv8832_direction {
	DRV8832_LEFT,
	DRV8832_RIGHT,
	DRV8832_UNKNOWN,
};

struct drv8832 {
	struct pca9557_pin left_out;
	struct pca9557_pin right_out;
	struct pca9557_pin left_in;
	struct pca9557_pin right_in;
};

struct drv8832 cell = {
	.left_out = { .address = 0x18, .pin_number = 7 },
	.right_out = { .address = 0x18, .pin_number = 6 },
	.left_in = { .address = 0x18, .pin_number = 5 },
	.right_in = { .address = 0x18, .pin_number = 4 }
};

struct drv8832 servo_2 = {
	.left_out = {.address = 0x18, .pin_number = 3},
	.right_out = {.address = 0x18, .pin_number = 2},
	.left_in = {.address = 0x18, .pin_number = 1},
	.right_in = {.address = 0x19, .pin_number = 7}
};

struct drv8832 servo_3 = {
	.left_out = {.address = 0x19, .pin_number = 6},
	.right_out = {.address = 0x19, .pin_number = 5},
	.left_in = {.address = 0x19, .pin_number = 4},
	.right_in = {.address = 0x19, .pin_number = 3}
};

struct drv8832 servo_4 = {
	.left_out = {.address = 0x19, .pin_number = 2},
	.right_out = {.address = 0x19, .pin_number = 1},
	.left_in = {.address = 0x1a, .pin_number = 7},
	.right_in = {.address = 0x1a, .pin_number = 6}
};

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	drv8832_init(cell);
	drv8832_init(servo_2);
	while (true)
	{
		drv8832_turn(cell, DRV8832_LEFT);
		delay_ms(1000);
		drv8832_turn(cell, DRV8832_RIGHT);
		delay_ms(1000);
		drv8832_turn(servo_2, DRV8832_LEFT);
		delay_ms(1000);
		drv8832_turn(servo_2, DRV8832_RIGHT);
		delay_ms(1000);
	}

}
