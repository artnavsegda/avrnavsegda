#include <avr/io.h>
#include "i2c.h"

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

void starti2c(void)
{
	TWIE.MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	TWIE.MASTER.BAUD = TWI_BAUD(2000000, 10000);
	TWIE.MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void i2csend(uint8_t ModuleAddress, uint8_t bytetosend)
{
	TWIE.MASTER.ADDR = ModuleAddress<<1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.DATA = bytetosend;//write word addr
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
}

uint8_t i2cread(uint8_t ModuleAddress)
{
	TWIE.MASTER.ADDR = (ModuleAddress<<1)+1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_RIF_bp);
	return TWIE.MASTER.DATA;
}
