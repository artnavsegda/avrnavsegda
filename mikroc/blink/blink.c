void main() {

     PORTR_DIR.B0 = 1;
           PORTR_DIR.B1 = 1;
                 PORTD_DIR.B4 = 1;
                       PORTD_DIR.B5 = 1;
          OSC_CTRL = 0x01;                 // 32MHz internal RC oscillator

          while(RC2MRDY_bit == 0)
              ;

     while(1)
     {
      //led0

      PORTR_OUTTGL.B0 = 1;
      delay_ms(500);
      //led1

      PORTR_OUTTGL.B1 = 1;
      delay_ms(500);
      //led2

      PORTD_OUTTGL.B4 = 1;
      delay_ms(500);
      //led3

      PORTD_OUTTGL.B5 = 1;
      delay_ms(500);
     }
}