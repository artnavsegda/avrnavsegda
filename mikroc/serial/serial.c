void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {

     char uart_rd;

     OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
     while(RC32MRDY_bit == 0);
     CPU_CCP = 0xD8;
     CLK_CTRL = 1;

     UARTC0_Init(9600);               // Initialize UART module at 9600 bps
     Delay_ms(100);                  // Wait for UART module to stabilize

     UARTC0_Write_Text("Hello world from here!\r\n");
     PrintOut(PrintHandler, "Testing output\r\n");
//  UARTC0_Write(13);
//  UARTC0_Write(10);

  while (1) {                     // Endless loop
   if (UARTC0_Data_Ready()) {      // If data is received,
     uart_rd = UARTC0_Read();      // read the received data,
     UARTC0_Write(uart_rd);        // and send data via UART
    }
  }
}
