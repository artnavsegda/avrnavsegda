#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

int main(void)
{
	int16_t result = 0;
	startserial();
	printf("MCU started\n\r");
	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		ADCA.CH0.CTRL |= ADC_CH_START_bm;
		while(!ADCA.CH0.INTFLAGS);
		result = ADCA.CH0RES;
		printf("adc: %d\n\r",result);
		//_delay_ms(1000);
	}
	return 0;
}
