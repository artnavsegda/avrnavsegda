void PrintHandler(char c)
{
        UARTC0_Write(c);
}

void main() {
     int i;
     
        OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
        while(RC32MRDY_bit == 0)
              ;
        CPU_CCP = 0xD8;
        CLK_CTRL = 1;
     
     UARTC0_Init(115200);               // Initialize UART module at 9600 bps
     Delay_ms(100);                  // Wait for UART module to stabilize

     //UARTC0_Write_Text("Hello world");
     PrintOut(PrintHandler, "Hello world\r\n");
     
     while(1)
     {
             Delay_ms(100);
             for (i=0;i<7;i++)
                 PrintOut(PrintHandler, "ADCA%d: %d ", i, ADCA_Read(i));
             for (i=0;i<7;i++)
                 PrintOut(PrintHandler, "ADCB%d: %d ", i, ADCB_Read(i));
             UARTC0_Write_Text("\r\n");
     }
}