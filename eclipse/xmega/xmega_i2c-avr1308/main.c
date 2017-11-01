#include <avr/io.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "twi_master_driver.h"

TWI_Master_t twiMaster;

int main(void)
{
	TWI_MasterInit(&twiMaster,&TWIE,TWI_MASTER_INTLVL_LO_gc,TWI_BAUD(F_CPU, 100000));
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();

	TWI_MasterWrite(&twiMaster, 0x18, "\x00\0xFF", 2)	;

	while(1);
}

/*! TWIC Master Interrupt vector. */
ISR(TWIC_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiMaster);
}
