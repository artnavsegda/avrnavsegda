#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TCC0_OVF_vect)
{
	PORTD_OUTTGL = _BV(4);
}

int main(void)
{
	PORTD_DIRSET = _BV(4);
	TCC0_PER = 0x2710;                // Set period 10000
	TCC0_CTRLA = 0x06;                // Prescaler DIV64
	TCC0_INTCTRLA = 2;                // Enable overflow interrupt
	PMIC_CTRL = 2;                    // Enable medium level interrupts
	sei();
	while(1);
}
