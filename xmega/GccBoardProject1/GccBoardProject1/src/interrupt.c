#include <asf.h>
#include <stdio.h>
#include "ad7705.h"
#include "interrupt.h"
#include "rolling.h"
#include "ra915.h"
#include "pca9557.h"
#include "i2c.h"

extern struct spi_device SPI_ADC;
extern sensor_t barometer;
int16_t adc_scan_results[16];
uint16_t adcdata;

void adc_callback(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			if (current_adca_scan_channel == 8)	current_adca_scan_channel = 0;
		}
	}
	else if (adc == &ADCB) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adcb_scan_channel+8] = result;
			current_adcb_scan_channel++;
			if (current_adcb_scan_channel == 8)	current_adcb_scan_channel = 0;
		}
	}
	adc_start_conversion(adc, ch_mask);
}

void ad7705_callback(void)
{
	LED_Toggle(LED1);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
}

void tc_callback(void)
{
	LED_Toggle(LED0);
	struct ra915struct frame = { .marker = 0xA5	};
	sensor_data_t press_data = { .scaled = true };
	sensor_data_t temp_data = { .scaled = false };

	sensor_get_pressure(&barometer, &press_data);
	sensor_get_temperature(&barometer, &temp_data);

	frame.data.pmt_current = adc_scan_results[0];
	frame.data.flow_rate = adc_scan_results[1];
	frame.data.pmt_voltage = adc_scan_results[2];
	frame.data.concentration = adcdata - 0x7FFF;
	frame.data.bypass_pressure = adc_scan_results[3];
	frame.data.t_analytical_cell = temp_data.temperature.value;
	frame.data.t_selftest_cell = adc_scan_results[4];
	frame.data.pressure_analytical_cell = press_data.pressure.value/10;
	frame.data.vacuum = adc_scan_results[5];
	frame.data.dilution_pressure = adc_scan_results[6];
	frame.data.status = generatestatusbyte();

	frame.checksum = genchecksum((uint8_t *)&frame.data,21);
	usart_serial_write_packet(&USARTC0, (uint8_t *)&frame, 23);

	//ra915frame(adcdata - 0x7FFF, adc_scan_results, press_data.pressure.value/10, temp_data.temperature.value);

	//printf(" %u ",press_data.pressure.value);

	/*static int i = 0;
	pca9557_toggle_pin_level(0x1a, i++);
	if (i == 8)
		i = 0;*/

	tc_clear_overflow(&TCC0);
}

void usart_callback(void)
{
	static bool control;
	LED_Toggle(LED0);
	uint8_t i = usart_getchar(&USARTC0);
	if (i == 0xB5)
	{
		control = true;
		usart_putchar(&USARTC0,i);
	}
	else if (control == true)
	{
		processcontrolbyte(i);
		control = false;
	}
	else
		control = false;
	usart_clear_rx_complete(&USARTC0);
}
