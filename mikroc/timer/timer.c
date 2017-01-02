extern int tick;

void main() {
	OSC_CTRL = 0x02;
 	while(RC32MRDY_bit == 0);
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;


	PORTD_DIRSET.B4 = 1;              // Set PORTD as output
	PMIC_CTRL = 4;                    // Enable medium level interrupts
	CPU_SREG.B7 = 1;                  // Enable global interrupts

	Timer_Init(&TCC0, 1000000);
	Timer_Interrupt_Enable(&TCC0);

//  TCC0_PERL = 0x10;                 // Set period 10000
//  TCC0_PERH = 0x27;
//  TCC0_CTRLA = 0x05;                // Prescaler DIV64
//  TCC0_INTCTRLA = 2;                // Enable overflow interrupt*/

        while (1)                         // Endless loop, port is changed inside Interrupt Service Routine (ISR)
        {
                if (tick == 1)
                {
                        PORTD_OUTTGL.B4 = 1;
                        tick = 0;
                }
        }
}