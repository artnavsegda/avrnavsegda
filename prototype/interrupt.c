void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF {
  PORTD_OUTTGL = 0xFF;             // Toggle PORTD
}
