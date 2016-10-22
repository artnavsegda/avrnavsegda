#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "drv8832.h"

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

enum drv8832_direction drv8832_get_direction(struct drv8832 servo)
{
	if (pca9557_get_pin_level(servo.left_in.address,servo.left_in.pin_number) && !pca9557_get_pin_level(servo.right_in.address,servo.right_in.pin_number))
	return DRV8832_LEFT;
	else if (!pca9557_get_pin_level(servo.left_in.address,servo.left_in.pin_number) && pca9557_get_pin_level(servo.right_in.address,servo.right_in.pin_number))
	return DRV8832_RIGHT;
	else
	return DRV8832_UNKNOWN;
}

void drv8832_turn(struct drv8832 servo, enum drv8832_direction direction)
{
	switch (direction)
	{
		case DRV8832_LEFT:
		if (drv8832_get_direction(servo) != DRV8832_LEFT)
		{
			pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, false);
			pca9557_set_pin_level(servo.left_out.address,servo.left_out.pin_number, true);
		}
		break;
		case DRV8832_RIGHT:
		if (drv8832_get_direction(servo) != DRV8832_RIGHT)
		{
			pca9557_set_pin_level(servo.left_out.address,servo.left_out.pin_number, false);
			pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, true);
		}
		break;
		default:
		break;
	}
}

void drv8832_init(struct drv8832 servo)
{
	pca9557_set_pin_dir(servo.left_out.address, servo.left_out.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(servo.right_out.address, servo.right_out.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(servo.left_in.address, servo.left_in.pin_number, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(servo.right_in.address, servo.right_in.pin_number, PCA9557_DIR_INPUT);
}

