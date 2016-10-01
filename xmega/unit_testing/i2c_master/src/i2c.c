#include <asf.h>
#include "i2c.h"

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content)
{
	uint8_t message[1];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 1
	};
	message[0] = content;
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content)
{
	uint8_t message[2];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 2
	};
	message[0] = LSB(content);
	message[1] = MSB(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_double(TWI_t *twi, uint8_t addr, uint8_t memory, float content)
{
	uint8_t message[4];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 4
	};
	message[0] = LSB0(content);
	message[1] = LSB1(content);
	message[2] = LSB2(content);
	message[3] = LSB3(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array)
{
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = array,
		.length       = arraysize
	};
	return twi_master_write(twi, &packet);
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	uint8_t message[1];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 1
	};
	status_code_t status = twi_master_read(twi, &packet);
	if(status == TWI_SUCCESS)
		return message[0];
	else
		return status;
}

uint16_t i2c_read_word(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	uint16_t recievedword;
	uint8_t message[2];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 2
	};
	status_code_t status = twi_master_read(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		LSB(recievedword) = message[0];
		MSB(recievedword) = message[1];
		return recievedword;
	}
	else
		return status;
}

uint32_t i2c_read_double(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	uint32_t recievedword;
	uint8_t message[4];
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = message,
		.length       = 4
	};
	status_code_t status = twi_master_read(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		MSB0(recievedword) = message[0];
		MSB1(recievedword) = message[1];
		MSB2(recievedword) = message[2];
		MSB3(recievedword) = message[3];
		return recievedword;
	}
	else
		return status;
}

status_code_t i2c_read_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array)
{
	twi_package_t packet = {
		.addr_length  = 1,
		.addr[0]      = memory,
		.chip         = addr,
		.buffer       = array,
		.length       = arraysize
	};
	return twi_master_read(twi, &packet);
}
