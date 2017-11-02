#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr_compiler.h"
#include "TC_driver.h"

ISR(TCC0_OVF_vect)
{
	PORTD_OUTTGL = _BV(3);
}

int main(void)
{
	PORTD_DIRSET = _BV(3);
	TC_SetPeriod( &TCC0, 20000 );
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV1_gc);
	TC0_SetOverflowIntLevel(&TCC0, TC_CCAINTLVL_MED_gc);
	PMIC_CTRL = PMIC_MEDLVLEN_bm;
	sei();
	while(1);
}


