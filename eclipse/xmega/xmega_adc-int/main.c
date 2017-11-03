#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

int16_t result = 0;

ISR(ADCA_CH0_vect)
{
	result = ADCA.CH0RES;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;
}

int main(void)
{
	startserial();
	printf("MCU started\n\r");
	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.INTCTRL |= ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEX_bm;
	sei();
	ADCA.CTRLA |= ADC_ENABLE_bm|ADC_CH_START_bm;

	//ADCA.CTRLB |= ADC_FREERUN_bm;

	while(1)
	{
		//ADCA.CH0.CTRL |= ADC_CH_START_bm;
		//while(!ADCA.CH0.INTFLAGS);
		//result = ADCA.CH0RES;
		printf("adc: %d\n\r",result);
		_delay_ms(1000);
	}
	return 0;
}
