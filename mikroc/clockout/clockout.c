void main() {
     PORTC_DIR.B7 = 1;
     PORTCFG_CLKEVOUT = 0x01;
     
     while (1)
           ;
     
}