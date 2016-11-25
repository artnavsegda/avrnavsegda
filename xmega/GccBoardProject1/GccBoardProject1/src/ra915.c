#include <asf.h>
#include "ra915.h"
#include "pca9557.h"
#include "drv8832.h"

void processcontrolbyte(uint8_t controlbyte)
{
	pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, controlbyte & _BV(1));//ign
	pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, controlbyte & _BV(2));
	drv8832_turn(&cell,controlbyte & _BV(3));
	pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, controlbyte & _BV(4));
	pca9557_set_pin_level(servo.right_out.address,servo.right_out.pin_number, controlbyte & _BV(5));
}

uint8_t generatestatusbyte(void)
{
	uint8_t statusbyte = 0;
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(1);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(2);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(3);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(4);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(5);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(6);
	if (pca9557_get_pin_level(servo.right_out.address,servo.right_out.pin_number)) statusbyte |= _BV(7);
	return statusbyte;
}

uint8_t genchecksum(uint8_t *massive, int sizeofmassive)
{
	uint8_t checksum = 0;
	for (int i=0;i<sizeofmassive;i++)
		checksum = checksum + massive[i];
	return checksum;
}