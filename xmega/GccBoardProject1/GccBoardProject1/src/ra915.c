#include <asf.h>
#include "ra915.h"
#include "pca9557.h"
#include "drv8832.h"

struct drv8832 cell = {
	.left_out = { .address = 0x18, .pin_number = 7 },
	.right_out = { .address = 0x18, .pin_number = 6 },
	.left_in = { .address = 0x18, .pin_number = 5 },
	.right_in = { .address = 0x18, .pin_number = 4 }
};

struct pca9557_pin elemental = { .address = 0x1a, .pin_number = 0 };
struct pca9557_pin calibration = { .address = 0x1a, .pin_number = 1 };
struct pca9557_pin zero = { .address = 0x1a, .pin_number = 2 };
struct pca9557_pin ignition = { .address = 0x1a, .pin_number = 3 };

struct pca9557_pin watlow1 = { .address = 0x18, .pin_number = 7 };
struct pca9557_pin watlow2 = { .address = 0x18, .pin_number = 7 };
struct pca9557_pin watlow3 = { .address = 0x18, .pin_number = 7 };
struct pca9557_pin watlow4 = { .address = 0x18, .pin_number = 7 };
struct pca9557_pin watlow5 = { .address = 0x18, .pin_number = 7 };

void ra915init(void)
{
	drv8832_init(cell);
	pca9557_set_pin_dir(elemental.address, elemental.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(calibration.address, calibration.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(zero.address, zero.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(ignition.address, ignition.pin_number, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(watlow1.address, watlow1.pin_number, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(watlow2.address, watlow2.pin_number, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(watlow3.address, watlow3.pin_number, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(watlow4.address, watlow4.pin_number, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(watlow5.address, watlow5.pin_number, PCA9557_DIR_INPUT);
}

void ra915recieve(void)
{
	uint8_t frame[5];
	uint8_t controlbyte;
	if (usart_getchar(&USARTC0) == 0xB5)
	{
		LED_Toggle(LED2);
		irqflags_t flags = cpu_irq_save();
		usart_serial_read_packet(&USARTC0,(uint8_t *)&frame,4);
		cpu_irq_restore(flags);
		if (frame[0] == frame[3])
		{
			usart_serial_putchar(&USARTC0,0xB5);
			controlbyte = frame[1];
			processcontrolbyte(controlbyte);
		}
	}
}

void processcontrolbyte(uint8_t controlbyte)
{
	pca9557_set_pin_level(ignition.address,ignition.pin_number,controlbyte & _BV(1)); // ign
	pca9557_set_pin_level(zero.address,zero.pin_number, controlbyte & _BV(2)); //zero
	drv8832_turn(cell,controlbyte & _BV(3));
	pca9557_set_pin_level(calibration.address,calibration.pin_number, controlbyte & _BV(4)); //cal
	pca9557_set_pin_level(elemental.address,elemental.pin_number, controlbyte & _BV(5)); //elemental
}

uint8_t generatestatusbyte(void)
{
	uint8_t statusbyte = 0;
	if (pca9557_get_pin_level(watlow1.address,watlow1.pin_number)) statusbyte |= _BV(1);//watlow1//internal
	if (pca9557_get_pin_level(cell.left_in.address,cell.left_in.pin_number)) statusbyte |= _BV(2);//cellon
	if (pca9557_get_pin_level(cell.right_in.address,cell.right_in.pin_number)) statusbyte |= _BV(3);//celloff
	if (pca9557_get_pin_level(watlow2.address,watlow2.pin_number)) statusbyte |= _BV(4);//watlow2//sample
	if (pca9557_get_pin_level(watlow3.address,watlow3.pin_number)) statusbyte |= _BV(5);//watlow3//probe
	if (pca9557_get_pin_level(watlow4.address,watlow4.pin_number)) statusbyte |= _BV(6);//watlow4//filter
	if (pca9557_get_pin_level(watlow5.address,watlow5.pin_number)) statusbyte |= _BV(7);//watlow5//converter
	return statusbyte;
}

uint8_t genchecksum(uint8_t *massive, int sizeofmassive)
{
	uint8_t checksum = 0;
	for (int i=0;i<sizeofmassive;i++)
		checksum = checksum + massive[i];
	return checksum;
}
