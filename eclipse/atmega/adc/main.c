#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "serial.h"

uint16_t adc_read(void)
{
	ADMUX = (0x1 << REFS0) | 0x0; // ADC_VREF_AVCC, ADC_MUX_ADC0
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));
	return (ADC);
}

int main(void)
{
	startserial();
	printf("MCU started\n\r");
	ADCSRA = _BV(ADEN) | (0x7 << ADPS0); // ADC_PRESCALER_DIV128

	while(1)
	{
		_delay_ms(1000);
		printf("adc read %d\r\n",adc_read());
	}
	return 0;
}
