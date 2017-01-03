#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/serial/serial.c"
void PrintHandler(char c)
{
 UARTC0_Write(c);
}

void main() {

 char uart_rd;

 OSC_CTRL = 0x02;
 while(RC32MRDY_bit == 0);
 CPU_CCP = 0xD8;
 CLK_CTRL = 1;

 UARTC0_Init(9600);
 Delay_ms(100);

 UARTC0_Write_Text("Hello world from here!\r\n");
 PrintOut(PrintHandler, "Testing output\r\n");



 while (1) {
 if (UARTC0_Data_Ready()) {
 uart_rd = UARTC0_Read();
 UARTC0_Write(uart_rd);
 }
 }
}
