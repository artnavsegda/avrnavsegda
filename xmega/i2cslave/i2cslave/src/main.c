/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

#define DATA_LENGTH 2
#define TWI_SPEED 50000

TWI_Slave_t slave;

uint8_t data[DATA_LENGTH] = { 0x0f, 0x1f };
uint8_t recv_data[DATA_LENGTH] = { 0x00, 0x00 };

ISR(TWIE_TWIS_vect)
{
	TWI_SlaveInterruptHandler(&slave);
}

static void slave_process(void)
{
	//int i;

	slave.sendData[0] = 0x0f;
	if (slave.receivedData[0] == 0x01)
		slave.sendData[0] = 0x11;
	if (slave.receivedData[0] == 0x02)
		slave.sendData[0] = 0x12;

	//for (i = 0; i < DATA_LENGTH; i++)
	//{
	//	recv_data[i] = slave.receivedData[i];
	//	slave.sendData[i] = data[i];
	//}
}

twi_slave_options_t opt = {
	.speed = 50000,
	.chip = 0x09,
};

int main (void)
{
	uint8_t i;
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	irq_initialize_vectors();
	twi_slave_setup(&TWIE,&opt,&slave,*slave_process,0x09,TWI_SLAVE_INTLVL_MED_gc);
	for (i = 0; i < TWIS_SEND_BUFFER_SIZE; i++)
	{
		slave.receivedData[i] = 0;
	}
	cpu_irq_enable();

	for (i = 0; i < DATA_LENGTH; i++)
	{
		slave.sendData[i] = data[i];
	}

	/* Insert application code here, after the board has been initialized. */
	do {
		// nothing
	} while (true);
}
