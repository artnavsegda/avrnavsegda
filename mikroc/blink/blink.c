void main() {
     while(1)
     {
      //led0
      PORTR_DIR.B0 = 1;
      PORTR_OUTTGL.B0 = 1;
      delay_ms(500);
      //led1
      PORTR_DIR.B1 = 1;
      PORTR_OUTTGL.B1 = 1;
      delay_ms(500);
      //led2
      PORTD_DIR.B4 = 1;
      PORTD_OUTTGL.B4 = 1;
      delay_ms(500);
      //led3
      PORTD_DIR.B5 = 1;
      PORTD_OUTTGL.B5 = 1;
      delay_ms(500);
     }
}