#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/spi/spi.c"
void main() {
 int x, y;

 delay_ms(100);

 UARTC0_Init(9600);

 delay_ms(100);

 UARTC0_Write_Text("hello");

 delay_ms(100);

 SPIC_Init();

 delay_ms(100);

 SPIC_Write(0x20);
 SPIC_Write(0x0C);
 SPIC_Write(0x10);
 SPIC_Write(0x04);

 while(1)
 {

 delay_ms(100);

 SPIC_Write(0x38);
 x = SPIC_Read(0xFF);
 y = SPIC_Read(0xFF);

 delay_ms(100);

 UARTC0_Write(x);
 UARTC0_Write(y);
 }
}
