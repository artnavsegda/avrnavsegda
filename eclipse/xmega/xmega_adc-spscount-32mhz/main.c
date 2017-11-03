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

int main(void)
{
    OSC.CTRL = 0x02;
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));
    CPU_CCP = 0xD8;
    CLK.CTRL = 1;

	startserial(207);//9600
	printf("MCU started\n\r");
	ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.INTCTRL |= ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEX_bm;
	sei();
	ADCA.CTRLB |= ADC_FREERUN_bm;
	ADCA.CTRLA |= ADC_ENABLE_bm;

	while(1)
	{
		printf("samples per second: %d\n\r",counter);
		counter = 0;
		_delay_ms(1000);
	}
	return 0;
}
