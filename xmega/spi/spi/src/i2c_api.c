/*
 * i2c_api.c
 *
 * Created: 30-Jun-16 7:56:07
 *  Author: artna
 */ 

#include <asf.h>
#include "i2c_api.h"

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content) // ??????? i2c ??????
{
	 status_code_t status;
	 uint8_t message[2];
	 twi_package_t packet = {
		 .chip         = addr,      // TWI slave bus address
		 .buffer       = message, // transfer data source buffer
		 .length       = 2  // transfer data size (bytes)
	 };
	 message[0] = memory;
	 message[1] = content;
	 status = twi_master_write(twi, &packet);
	 return status;
}

status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content) // ??????? i2c ??????
{
	status_code_t status;
	uint8_t message[3];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 3  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = LSB(content);
	message[2] = MSB(content);
	status = twi_master_write(twi, &packet);
	return status;
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	 status_code_t status;
	 uint8_t message[1];
	 twi_package_t packet = {
		 .chip         = addr,      // TWI slave bus address
		 .buffer       = message,        // transfer data destination buffer
		 .length       = 1                    // transfer data size (bytes)
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
		.chip         = addr,      // TWI slave bus address
		.buffer       = message,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
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