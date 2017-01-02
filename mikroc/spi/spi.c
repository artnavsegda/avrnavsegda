sbit Chip_Select at PORTC_OUT.B4;
sbit Chip_Select_Direction at PORTC_DIR.B4;


void main() {
	int x, y;
 	Chip_Select_Direction = 1; // ad7705 cs
        delay_ms(100);

        UARTC0_Init(9600);

        delay_ms(100);

        UARTC0_Write_Text("hello");

        delay_ms(100);

        SPIC_Init();

        delay_ms(100);
        
        Chip_Select = 0;
        SPIC_Write(0x20);
        SPIC_Write(0x0C);
        SPIC_Write(0x10);
        SPIC_Write(0x04);
        Chip_Select = 1;
     
	while(1)
	{
		delay_ms(100);
		
                Chip_Select = 1;
                SPIC_Write(0x38);
                x = SPIC_Read(0xFF);
                y = SPIC_Read(0xFF);
                Chip_Select = 0;
                
                delay_ms(100);
       
		UARTC0_Write(x);
		UARTC0_Write(y);
	}
}