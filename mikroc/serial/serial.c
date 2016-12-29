void main() {

char uart_rd;

  UARTC0_Init(9600);               // Initialize UART module at 9600 bps
  Delay_ms(100);                  // Wait for UART module to stabilize

  UARTC0_Write_Text("Init");
  UARTC0_Write(13);
  UARTC0_Write(10);

  while (1) {                     // Endless loop
   if (UARTC0_Data_Ready()) {      // If data is received,
     uart_rd = UARTC0_Read();      // read the received data,
     UARTC0_Write(uart_rd);        // and send data via UART
    }
  }
}