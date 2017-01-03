#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/spi/spi.c"


sbit Chip_Select at PORTC_OUT.B4;

sbit ad7707_drdy at PORTC_IN.B1;


sfr sbit SPI_Ethernet_CS at PORTC_OUT.B0;

void PrintHandler(char c)
{
 UARTC0_Write(c);
}

void SPIC_Read_Bytes(char *buffer, unsigned NoBytes)
{
 int i;
 for (i = 0; i < NoBytes; i++)
 buffer[i] = SPIC_Read(0xFF);
}

void main() {
 int x, y;
 unsigned int result;


 PORTD_DIR.B5 = 1;
 delay_ms(100);

 UARTC0_Init(115200);

 delay_ms(100);

 UARTC0_Write_Text("hello\r\n");

 delay_ms(100);

 SPIC_Init();

 delay_ms(100);

 SPI_Ethernet_CS = 1;

 Chip_Select = 0;

 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);
 SPIC_Write(0xFF);

 delay_ms(100);

 SPIC_Write(0x20);
 SPIC_Write(0x0C);
 SPIC_Write(0x10);
 SPIC_Write(0x04);

 delay_ms(100);

 SPIC_Write(0x60);
 SPIC_Write(0x18);
 SPIC_Write(0x3A);
 SPIC_Write(0x00);

 delay_ms(100);

 SPIC_Write(0x70);
 SPIC_Write(0x89);
 SPIC_Write(0x78);
 SPIC_Write(0xD7);
 Chip_Select = 1;

 while(1)
 {




 if (ad7707_drdy == 0)




 {
 PORTD_OUTTGL.B5 = 1;
 Chip_Select = 0;
 SPIC_Write(0x38);




 SPIC_Read_Bytes((unsigned char *)&result,2);
 Chip_Select = 1;
 }




 PrintOut(PrintHandler, "cs0 %x\r\n",  ((((result) >> 8) & 0xff) | (((result) & 0xff) << 8)) );



 }
}
