#include <asf.h>
#include "i2c.h"

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content)
{
	uint8_t message[2];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 2
	};
	message[0] = memory;
	message[1] = content;
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content)
{
	uint8_t message[3];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 3
	};
	message[0] = memory;
	message[1] = MSB(content);
	message[2] = LSB(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_double(TWI_t *twi, uint8_t addr, uint8_t memory, uint32_t content)
{
	uint8_t message[5];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 5
	};
	message[0] = memory;
	message[1] = MSB0(content);
	message[2] = MSB1(content);
	message[3] = MSB2(content);
	message[4] = MSB3(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array)
{
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = array,
		.length       = arraysize
	};
	return twi_master_write(twi, &packet);
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	status_code_t status;
	uint8_t message[1];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 1
	};
	message[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		status_code_t status = twi_master_read(twi, &packet);
		if(status == TWI_SUCCESS)
		return message[0];
	}
	return status;
}

uint16_t i2c_read_word(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	status_code_t status;
	uint16_t recievedword;
	uint8_t message[2];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 1
	};
	message[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		packet.length = 2;
		status_code_t status = twi_master_read(twi, &packet);
		if(status == TWI_SUCCESS)
		{
			LSB(recievedword) = message[0];
			MSB(recievedword) = message[1];
			return recievedword;
		}
	}
	return status;
}

uint32_t i2c_read_double(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	status_code_t status;
	uint32_t recievedword;
	uint8_t message[4];
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = message,
		.length       = 1
	};
	message[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		packet.length = 4;
		status_code_t status = twi_master_read(twi, &packet);
		if(status == TWI_SUCCESS)
		{
			MSB0(recievedword) = message[0];
			MSB1(recievedword) = message[1];
			MSB2(recievedword) = message[2];
			MSB3(recievedword) = message[3];
			return recievedword;
		}
	}
	return status;
}

status_code_t i2c_read_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array)
{
	status_code_t status;
	twi_package_t packet = {
		.chip         = addr,
		.buffer       = array,
		.length       = 1
	};
	((uint8_t *)array)[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		packet.length = arraysize;
		return twi_master_read(twi, &packet);
	}
	return status;
}
