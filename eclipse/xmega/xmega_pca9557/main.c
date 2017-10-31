#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)

char ModuleAddress = 0x18;

void Expander_Init(char ModuleAddress)
{
        TWIE_Init(100000);
        Expander_Write_Byte(ModuleAddress,0x02,0x00);
}

void TWIE_Init(int twibaud)
{
	TWIE.MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	TWIE.MASTER.BAUD = TWI_BAUD(F_CPU, twibaud);
	TWIE.MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

char TWIE_Read(char ModuleAddress)
{
	TWIE.MASTER.ADDR = ModuleAddress<<1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.DATA = 0x00;//write word addr
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_WIF_bp);
	TWIE.MASTER.ADDR = (ModuleAddress<<1)+1;
	loop_until_bit_is_set(TWIE.MASTER.STATUS, TWI_MASTER_RIF_bp);
	TWIE.MASTER.CTRLC = TWI_MASTER_ACKACT_bm;
	return TWIE.MASTER.DATA;
}

void Expander_Write_Byte(char ModuleAddress, char Register, char WriteData)
{

}

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

