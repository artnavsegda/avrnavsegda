#include <asf.h>

void setup_init(void)
{
	twi_options_t m_options = {
		.speed     = 50000,
		.chip      = 0x50,
		.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), 50000)
	};

	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
	twi_master_init(&TWIC, &m_options);
}

void twi_configure(void)
{
	twi_master_options_t opt = {
		.speed = 50000,
		.chip  = 0x50
	};
	twi_master_setup(&TWIC, &opt);
}

void setup_configure(void)
{
	twi_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIC);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
}

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content) // ??????? i2c ??????
{
	uint8_t message[2];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 2  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = content;
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content) // ??????? i2c ??????
{
	uint8_t message[3];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 3  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = MSB(content);
	message[2] = LSB(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_double(TWI_t *twi, uint8_t addr, uint8_t memory, uint32_t content) // ??????? i2c ??????
{
	uint8_t message[5];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 5  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = MSB0(content);
	message[2] = MSB1(content);
	message[3] = MSB2(content);
	message[4] = MSB3(content);
	return twi_master_write(twi, &packet);
}

status_code_t i2c_send_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, uint8_t array[]) // ??????? i2c ??????
{
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = array, // transfer data source buffer
		.length       = arraysize  // transfer data size (bytes)
	};
	return twi_master_write(twi, &packet);
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

uint32_t i2c_read_double(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	status_code_t status;
	uint32_t recievedword;
	uint8_t message[4];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
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

status_code_t i2c_read_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, uint8_t array[])
{
	status_code_t status;
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = array,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
	};
	array[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		packet.length = arraysize;
		return twi_master_read(twi, &packet);
	}
	return status;
}
