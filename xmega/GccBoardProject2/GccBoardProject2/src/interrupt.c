#include <asf.h>
#include "interrupt.h"
#include "sequencer.h"
#include "spi_transfer.h"

static volatile int16_t adca_scan_results[8];
static volatile int16_t adcb_scan_results[8];
uint16_t ad7705_raw_data, ad7705_averaged_data;

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

void sequence_callback(void)
{
	decrement_mode_counter();
	//increment(measurment_averaging_massive, average(ad7705_averaging_massive, AD7705_AVERAGING_AMOUNT);
	//increment(temperature_averaging_massive, current_temperature);
}

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
	uint8_t array[2];
	spi_select_device(&SPIC, &SPI_ADC);
	spi_transfer(&SPIC, 0x08);
	if (spi_transfer(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_transfer(&SPIC,0x38);
		spi_read_packet(&SPIC, array, 2);
	}
	spi_deselect_device(&SPIC, &SPI_ADC);
		//if (ad7705_data_is_ready())
		//	increment(ad7705_averaging_massive, ad7705_recieve_value());
}

ISR(PORTC_INT0_vect)
{

}

ISR(PORTE_INT0_vect)
{

}