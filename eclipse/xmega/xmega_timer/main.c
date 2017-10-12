#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TCC0_OVF_vect)
{
	PORTD_OUTTGL = _BV(3);
}

int main(void)
{
	PORTD_DIRSET = _BV(3);
	TCC0_PER = 20000;                // Set period 10000
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;                // Prescaler DIV1
	TCC0_INTCTRLA = 2;                // Enable overflow interrupt
	PMIC_CTRL = 2;                    // Enable medium level interrupts
	sei();
	while(1);
}
