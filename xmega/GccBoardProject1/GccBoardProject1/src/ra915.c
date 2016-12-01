#include <asf.h>
#include "ra915.h"
#include "pca9557.h"
#include "drv8832.h"

/*

asr[0] = ADCA0 = PA0 = J3-1 = AD7705_RESET
asr[1] = ADCA1 = PA1 = J3-2 = X11_6
asr[2] = ADCA2 = PA2 = J3-3 = X11_5
asr[3] = ADCA3 = PA3 = J3-4 = DISPLAY_RESET
asr[4] = ADCA4 = PA4 == ADC4 = J2-5 = VACUUM
asr[5] = ADCA5 = PA5 == ADC5 = J2-6 = DILUTION
asr[6] = ADCA6 = PA6 == ADC6 = J2-7 = BYPASS_PRESSURE
asr[7] = ADCA7 = PA7 == ADC7 = J2-8 = P4

asr[8] = ADCB0 = PB0 = ADC0 = J2-1 = PMT_CURR
asr[9] = ADCB1 = PB1 = ADC1 = J2-2 = PMT_V
asr[10] = ADCB2 = PB2 = ADC2 = J2-3 = FLOW
asr[11] = ADCB3 = PB3 = ADC3 = J2-4 = T_S_C
asr[12] = ADCB4 = PB4 = J3-5 = X11_4
asr[13] = ADCB5 = PB5 = J3-6 = X11_3
asr[14] = ADCB6 = PB6 = J3-7 = X11_2
asr[15] = ADCB7 = PB7 = J3-8 = X11_1

*/


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

void ra915frame(int ad7705adc, int16_t *internaladc, uint16_t pressure, uint16_t temperature)
{
	struct ra915struct frame = { .marker = 0xA5	};

	frame.data.pmt_current = internaladc[8];
	frame.data.flow_rate = internaladc[10];
	frame.data.pmt_voltage = internaladc[9];
	frame.data.concentration = ad7705adc;
	frame.data.bypass_pressure = internaladc[3];
	frame.data.t_analytical_cell = temperature;
	frame.data.t_selftest_cell = internaladc[4];
	frame.data.pressure_analytical_cell = pressure;
	frame.data.vacuum = internaladc[5];
	frame.data.dilution_pressure = internaladc[6];
	frame.data.status = generatestatusbyte();

	frame.checksum = genchecksum((uint8_t *)&frame.data,21);

	usart_serial_write_packet(&USARTC0, (uint8_t *)&frame, 23);
}
