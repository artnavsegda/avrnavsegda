#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(PORTA_INT0_vect)
{
	PORTA.OUTTGL = PIN0_bm;
}

int main(void)
{
	unsigned char x = 0;
	PORTA.DIR = PIN0_bm;
	PORTA.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTA.INT0MASK = PIN1_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTD.DIR = PIN4_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	while (1)
	{
		x = PORTA.IN;
		PORTD.OUTTGL = PIN4_bm;
		_delay_ms(1000);
	}
	return 0;
}

