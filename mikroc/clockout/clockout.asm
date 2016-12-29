
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;clockout.c,1 :: 		void main() {
;clockout.c,2 :: 		PORTC_DIR.B7 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 128
	STS        PORTC_DIR+0, R27
;clockout.c,3 :: 		PORTCFG_CLKEVOUT = 0x01;
	LDI        R27, 1
	STS        PORTCFG_CLKEVOUT+0, R27
;clockout.c,5 :: 		while (1)
L_main0:
;clockout.c,6 :: 		;
	JMP        L_main0
;clockout.c,8 :: 		}
L_end_main:
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
