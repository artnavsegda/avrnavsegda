#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

struct pca9557_pin ignition = {
	.address = 0x18,
	.pin_number = 6,
	.direction = PCA9557_DIR_OUTPUT
};

struct abc1234 cell = {
	.left_out.address = 0x18, .left_out.pin_number = 7,
	.right_out.address = 0x18, .right_out.pin_number = 6,
	.left_in.address = 0x18, left_in.pin_number = 5,
	.right_in.address = 0x18, right_in.pin_number 4,
};

struct abc1234 servo_2 = {
	.left_out.address = 0x18, .left_out.pin_number = 3,
	.right_out.address = 0x18, .right_out.pin_number = 2,
	.left_in.address = 0x18, left_in.pin_number = 1,
	.right_in.address = 0x19, right_in.pin_number 7,
};

struct abc1234 servo_3 = {
	.left_out.address = 0x19, .left_out.pin_number = 6,
	.right_out.address = 0x19, .right_out.pin_number = 5,
	.left_in.address = 0x19, left_in.pin_number = 4,
	.right_in.address = 0x19, right_in.pin_number 3,
};

struct abc1234 servo_4 = {
	.left_out.address = 0x19, .left_out.pin_number = 2,
	.right_out.address = 0x19, .right_out.pin_number = 1,
	.left_in.address = 0x1a, left_in.pin_number = 7,
	.right_in.address = 0x1a, right_in.pin_number 6,
};

abc1234_direction abc1234_get_direction(struct abc1234 servo)
{
	if (pca9557_get_pin_level(servo.left_in.address,servo.left_in.pin_number) && !pca9557_get_pin_level(servo.right_in.address,servo.right_in.pin_number))
		return ABC1234_LEFT;
	else if (!pca9557_get_pin_level(servo.left_in.address,servo.left_in.pin_number) && pca9557_get_pin_level(servo.right_in.address,servo.right_in.pin_number))
		return ABC1234_RIGHT;
	else
		return ABC1234_UNKNOWN;
}

void abc1234_turn(struct abc1234 servo, abc1234_direction direction)
{
	switch (direction)
	{
		case ABC1234_LEFT:
			if (abc1234_get_direction(servo) != ABC1234_LEFT)
			{
				pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, false);
				pca9557_set_pin_level(servo.left_out.address,servo.left_out.pin_number, true);
			}
		break;
		case ABC1234_RIGHT:
			if (abc1234_get_direction(servo) != ABC1234_RIGHT)
			{
				pca9557_set_pin_level(servo.left_out.address,servo.left_out.pin_number, false);
				pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, true);
			}
		break;
		default:
		break;
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	pca9557_set_pin_dir(ignition.address, ignition.pin_number, ignition.direction);

	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 3, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 4, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 5, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 6, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 7, PCA9557_DIR_OUTPUT);

	pca9557_set_pin_dir(ignition.address, ignition.pin_number, true);
	delay_ms(1000);
	pca9557_set_pin_dir(ignition.address, ignition.pin_number, false);

	pca9557_set_pin_level(0x1a, 0, false);
	pca9557_set_pin_level(0x1a, 1, true);
	pca9557_set_pin_level(0x1a, 2, false);
	pca9557_set_pin_level(0x1a, 3, true);
	pca9557_set_pin_level(0x1a, 4, false);
	pca9557_set_pin_level(0x1a, 5, true);
	pca9557_set_pin_level(0x1a, 6, false);
	pca9557_set_pin_level(0x1a, 7, true);

	abc1234_turn(cell, ABC1234_LEFT);
	delay_ms(1000);
	abc1234_turn(cell, ABC1234_RIGHT);

}
