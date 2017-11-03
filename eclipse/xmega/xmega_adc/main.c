#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"

uint16_t average(int amount)
{
	unsigned long temporary = 0;
	for (int i=0; i<amount;i++)
	{
		ADCB.CH0.CTRL |= ADC_CH_START_bm;
		while(!ADCB.CH0.INTFLAGS);
		temporary += ADCB.CH0RES;
	}
	return temporary/amount;
}

int main(void)
{
	int16_t result = 0;
	startserial();
	printf("MCU started\n\r");
	ADCB.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB.PRESCALER |= ADC_PRESCALER_DIV512_gc;
	ADCB.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCB.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCB.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN3_gc;
	ADCB.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		printf("adc: %d\n\r",average(16));
		_delay_ms(1000);
	}
	return 0;
}
