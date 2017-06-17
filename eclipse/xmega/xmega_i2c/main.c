#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

char ModuleAddress = 32;

int main(void)
{
	startserial();
	//twi init
	TWIC.MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	TWIC.MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	TWIC.MASTER.BAUD = TWI_BAUD(2000000, 100000);
	TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	while (1)
	{
		//twi address
		TWIC.MASTER.ADDR = (ModuleAddress<<1)+1;
		loop_until_bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_WIF_bp);
		//twi data
		TWIC.MASTER.DATA = 0x00;//write word addrpointer first
		TWIC.MASTER.ADDR = ModuleAddress<<1;
		loop_until_bit_is_set(TWIC.MASTER.STATUS, TWI_MASTER_RIF_bp);
		printf("Data: %x\r\n", TWIC.MASTER.DATA);
		_delay_ms(1000);
	}
	return 0;
}

