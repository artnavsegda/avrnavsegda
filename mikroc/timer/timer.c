void Timer0Overflow_ISR() org IVT_ADDR_TCC0_OVF {
  PORTD_OUTTGL = 0xFF;             // Toggle PORTD
}

void main() {
  PORTD_DIRSET = 0xFF;              // Set PORTD as output
  PORTD_OUTCLR = 0xFF;              // Clear PORTD
  PMIC_CTRL = 4;                    // Enable medium level interrupts
  CPU_SREG.B7 = 1;                  // Enable global interrupts

  Timer_Init(&TCC0, 1000000);
  Timer_Interrupt_Enable(&TCC0);

//  delay_ms(1000);

//  TCC0_PERL = 0x10;                 // Set period 10000
//  TCC0_PERH = 0x27;
//  TCC0_CTRLA = 0x05;                // Prescaler DIV64
//  TCC0_INTCTRLA = 2;                // Enable overflow interrupt*/

  while (1)                         // Endless loop, port is changed inside Interrupt Service Routine (ISR)
    ;
}