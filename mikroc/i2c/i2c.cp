#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/i2c/i2c.c"
void main()
{
 TWIE_Init(100000);
 TWIE_Start();
 TWIE_Write(0x1a << 1);
 TWIE_Write(0x03);
 TWIE_Write(0x00);
 TWIE_Stop();

 Delay_100ms();

 TWIE_Init(100000);
 TWIE_Start();
 TWIE_Write(0x1a << 1);
 TWIE_Write(0x02);
 TWIE_Write(0x00);
 TWIE_Stop();

 while (1)
 {
 Delay_100ms();

 TWIE_Init(100000);
 TWIE_Start();
 TWIE_Write(0x1a << 1);
 TWIE_Write(0x01);
 TWIE_Write(0x00);
 TWIE_Stop();

 Delay_100ms();

 TWIE_Init(100000);
 TWIE_Start();
 TWIE_Write(0x1a << 1);
 TWIE_Write(0x01);
 TWIE_Write(0xFF);
 TWIE_Stop();
 }
}
