void sequence_callback(void)
{
	snprintf(string,sizeof(string),"%04X", adcdata);
	gfx_mono_draw_string(string,8,0,&sysfont);
	increment(&secondstage,oversample(firststage,64));
	increment(&adcmassive,adc_scan_results[0]);
	i2c_send_word(&TWIE, 0x08, 0, adcdata);
	i2c_send_word(&TWIE, 0x08, 1, oversample(firststage,64));
	i2c_send_word(&TWIE, 0x08, 2, oversample(secondstage,128));
	i2c_send_word(&TWIE, 0x08, 3, oversample(adcmassive,128));
}