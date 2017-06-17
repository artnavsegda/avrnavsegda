#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

char ModuleAddress = 0x18;

int main(void)
{
	startserial();
	//twi init
	TWIE.MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	TWIE.MASTER.BAUD = TWI_BAUD(2000000, 10000);
	TWIE.MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	while (1)
	{
		TWIE.MASTER.ADDR = ModuleAddress<<1;
		loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
		TWIE.MASTER.DATA = 0x00;//write word addr
		loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
		TWIE.MASTER.ADDR = (ModuleAddress<<1)+1;
		loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_RIF_bp);
		printf("Data: %x\r\n", TWIE.MASTER.DATA);
		TWIE.MASTER.CTRLC = TWI_MASTER_ACKACT_bm;
		_delay_ms(1000);
	}
	return 0;
}

