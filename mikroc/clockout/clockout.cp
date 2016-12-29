#line 1 "C:/Users/artna/Documents/GitHub/avrnavsegda/mikroc/clockout/clockout.c"
void main() {
 PORTC_DIR.B7 = 1;
 PORTCFG_CLKEVOUT = 0x01;

 while (1)
 ;

}
