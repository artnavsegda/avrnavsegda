#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "avr_compiler.h"
#include "adc_driver.h"
#include "serial.h"

int16_t result = 0;

ISR(ADCA_CH3_vect)
{
	result = ADC_ResultCh_GetWord_Signed(&ADCA.CH3, 0);
	ADC_Ch_Conversion_Start(&ADCA.CH3);
}

int main(void)
{
	startserial();
	printf("MCU started\n\r");
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);
	ADC_Reference_Config(&ADCA, ADC_REFSEL_INTVCC_gc);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0, ADC_CH_INPUTMODE_SINGLEENDED_gc, ADC_DRIVER_CH_GAIN_NONE);
	ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_MUXPOS_PIN4_gc, 0);

	ADC_Ch_Interrupts_Config(&ADCA.CH3, ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
	PMIC.CTRL |= PMIC_LOLVLEX_bm;
	sei();

	ADC_Enable(&ADCA);
	ADC_Ch_Conversion_Start(&ADCA.CH3);

	//ADC_FreeRunning_Enable(&ADCA);

	while(1)
	{
		//ADC_Ch_Conversion_Start(&ADCA.CH3);
		//while(!ADC_Ch_Conversion_Complete(&ADCA.CH3));
		//result = ADC_ResultCh_GetWord_Signed(&ADCA.CH3, 0);
		printf("adc: %d\n\r",result);
		_delay_ms(1000);
	}

}
