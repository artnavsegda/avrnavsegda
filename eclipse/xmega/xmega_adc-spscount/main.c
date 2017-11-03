#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

unsigned long counter = 0;

ISR(ADCA_CH0_vect)
{
	counter++;
}

ISR(TCC0_OVF_vect)
{
	printf("samples per second: %lu\n\r",counter);
	counter = 0;
}

int main(void)
{
	startserial();
	printf("MCU started\n\r");

	TCC0_PER = 31250;
	TCC0_CTRLA = TC_CLKSEL_DIV64_gc;
	TCC0_INTCTRLA = 2;

	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.INTCTRL |= ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEX_bm | PMIC_MEDLVLEX_bm;
	sei();
	ADCA.CTRLB |= ADC_FREERUN_bm;
	ADCA.CTRLA |= ADC_ENABLE_bm;

	while(1);

	return 0;
}
