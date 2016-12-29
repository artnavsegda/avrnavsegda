
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;spi.c,1 :: 		void main() {
;spi.c,5 :: 		SPIC_Init();
	PUSH       R2
	CALL       _SPIC_Init+0
;spi.c,6 :: 		SPIC_Write(0x1);
	LDI        R27, 1
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,7 :: 		x = SPIC_Read(0xFF);
	LDI        R27, 255
	MOV        R2, R27
	CALL       _SPIC_Read+0
;spi.c,8 :: 		}
L_end_main:
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
