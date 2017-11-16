#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

void starti2c(void)
{
	TWIE.MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	TWIE.MASTER.BAUD = TWI_BAUD(F_CPU, 10000);
	TWIE.MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

uint8_t pcaread(uint8_t ModuleAddress, uint8_t ModuleRegister)
{
	TWIE.MASTER.ADDR = ModuleAddress<<1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.DATA = ModuleRegister;//write word addr
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);

	TWIE.MASTER.ADDR = (ModuleAddress<<1)+1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_RIF_bp);
	uint8_t RegisterData = TWIE.MASTER.DATA;

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

