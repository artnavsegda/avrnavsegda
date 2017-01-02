int tick;

void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF {
	tick = 1;
}