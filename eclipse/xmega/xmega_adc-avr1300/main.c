#include <avr/io.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "adc_driver.h"

int main(void)
{
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);
	ADC_Reference_Config(&ADCA, ADC_REFSEL_INTVCC_gc);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0, ADC_CH_INPUTMODE_SINGLEENDED_gc, ADC_DRIVER_CH_GAIN_NONE);
}
