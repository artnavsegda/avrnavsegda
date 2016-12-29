#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/spi/spi.c"
void main() {

 int x;

 SPIC_Init();
 SPIC_Write(0x1);
 x = SPIC_Read(0xFF);
}
