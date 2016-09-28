#include <asf.h>
#include "interrupt.h"
#include "sequencer.h"
#include "spi_transfer.h"
#include "rolling.h"

int16_t adc_scan_results[16];
extern struct massive ad7705_averaging_massive;
extern struct spi_device SPI_ADC;

void ISR_init(void)
{
	PORTC.INT0MASK = PIN1_bm;
	PORTE.INT0MASK = PIN5_bm;
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc;
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
}

void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			if (current_adca_scan_channel == 8) current_adca_scan_channel = 0;
		}
	}
	else if (adc == &ADCB) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adcb_scan_channel+8] = result;
			current_adcb_scan_channel++;
			if (current_adcb_scan_channel == 8)	current_adcb_scan_channel = 0;
		}
	}
}

ISR(PORTC_INT0_vect)
{
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		increment(ad7705_averaging_massive,ad7705_get_data_register(&SPIC, &SPI_ADC));
}

ISR(PORTC_INT0_vect)
{

}

ISR(PORTE_INT0_vect)
{

}