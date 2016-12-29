void main() {

     int x;

     SPIC_Init();
     SPIC_Write(0x1);
     x = SPIC_Read(0xFF);
}