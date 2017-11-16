#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"
#include "i2c.h"

uint8_t pcaread(uint8_t ModuleAddress, uint8_t ModuleRegister)
{
	i2csend(0x18, ModuleRegister);//write word addr
	uint8_t RegisterData = i2cread(0x18);
	TWIE.MASTER.CTRLC = TWI_MASTER_ACKACT_bm;
	return RegisterData;
}

void pcawrite(uint8_t ModuleAddress, uint8_t ModuleRegister, uint8_t RegisterData)
{
	TWIE.MASTER.ADDR = ModuleAddress<<1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.DATA = ModuleRegister;//write word addr
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.DATA = RegisterData;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.CTRLC = TWI_MASTER_ACKACT_bm;
}

int main(void)
{
	startserial();
	starti2c();
	pcawrite(0x18,0x02,0x00);
	while (1)
	{
		printf("Data: %x\r\n", pcaread(0x18,0x0));
		_delay_ms(1000);
	}
	return 0;
}

