#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/serial/serial.c"
void main() {

char uart_rd;

 UARTC0_Init(9600);
 Delay_ms(100);

 UARTC0_Write_Text("Init");
 UARTC0_Write(13);
 UARTC0_Write(10);

 while (1) {
 if (UARTC0_Data_Ready()) {
 uart_rd = UARTC0_Read();
 UARTC0_Write(uart_rd);
 }
 }
}
