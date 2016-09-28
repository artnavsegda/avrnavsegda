#include <asf.h>
#include "spi_transfer.h"
#include "rolling.h"

extern struct spi_device SPI_ADC;
extern struct massive firststage;
extern int16_t adca_scan_results[8];
extern int16_t adcb_scan_results[8];

void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	// Store the ADC results from the scan in the result array
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adca_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			// When 8 pins have been scanned the SCAN OFFSET wraps to zero
			if (current_adca_scan_channel == 8) {
				current_adca_scan_channel = 0;
			}
		}
	}
	else if (adc == &ADCB) {
		if (ch_mask & ADC_CH0) {
			adcb_scan_results[current_adcb_scan_channel] = result;
			current_adcb_scan_channel++;
			// When 8 pins have been scanned the SCAN OFFSET wraps to zero
			if (current_adcb_scan_channel == 8) {
				current_adcb_scan_channel = 0;
			}
		}
	}
}

ISR(PORTC_INT0_vect)
{
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		increment(firststage,ad7705_get_data_register(&SPIC, &SPI_ADC));
}
