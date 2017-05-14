void main() {
     OSC_CTRL.RC32MEN = 1;
     OSC_CTRL.RC32KEN = 1;
     while(RC32KRDY_bit == 0);
     while(RC32MRDY_bit == 0);
     DFLLRC32M_CTRL.ENABLE = 1;
     CPU_CCP = 0xD8;
     CLK_CTRL.SCLKSEL0 = 1;
     OSC_CTRL.RC2MEN = 0;

/*          OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
          while(RC32MRDY_bit == 0);
          CPU_CCP = 0xD8;
          CLK_CTRL = 1;*/

     PORTC_DIR.B7 = 1;
     PORTCFG_CLKEVOUT = 0x01;
     
     while (1);
}