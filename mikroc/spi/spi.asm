
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;spi.c,1 :: 		void main() {
;spi.c,4 :: 		delay_ms(100);
	PUSH       R2
	PUSH       R3
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main0:
	DEC        R16
	BRNE       L_main0
	DEC        R17
	BRNE       L_main0
	DEC        R18
	BRNE       L_main0
	NOP
;spi.c,6 :: 		UARTC0_Init(9600);
	LDS        R16, USARTC0_CTRLB+0
	ANDI       R16, 251
	STS        USARTC0_CTRLB+0, R16
	LDI        R27, 12
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;spi.c,8 :: 		delay_ms(100);
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main2:
	DEC        R16
	BRNE       L_main2
	DEC        R17
	BRNE       L_main2
	DEC        R18
	BRNE       L_main2
	NOP
;spi.c,10 :: 		UARTC0_Write_Text("hello");
	LDI        R27, #lo_addr(?lstr1_spi+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr1_spi+0)
	MOV        R3, R27
	CALL       _UARTC0_Write_Text+0
;spi.c,12 :: 		delay_ms(100);
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main4:
	DEC        R16
	BRNE       L_main4
	DEC        R17
	BRNE       L_main4
	DEC        R18
	BRNE       L_main4
	NOP
;spi.c,14 :: 		SPIC_Init();
	CALL       _SPIC_Init+0
;spi.c,16 :: 		delay_ms(100);
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main6:
	DEC        R16
	BRNE       L_main6
	DEC        R17
	BRNE       L_main6
	DEC        R18
	BRNE       L_main6
	NOP
;spi.c,18 :: 		SPIC_Write(0x20);
	LDI        R27, 32
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,19 :: 		SPIC_Write(0x0C);
	LDI        R27, 12
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,20 :: 		SPIC_Write(0x10);
	LDI        R27, 16
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,21 :: 		SPIC_Write(0x04);
	LDI        R27, 4
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,23 :: 		while(1)
L_main8:
;spi.c,26 :: 		delay_ms(100);
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main10:
	DEC        R16
	BRNE       L_main10
	DEC        R17
	BRNE       L_main10
	DEC        R18
	BRNE       L_main10
	NOP
;spi.c,28 :: 		SPIC_Write(0x38);
	LDI        R27, 56
	MOV        R2, R27
	CALL       _SPIC_Write+0
;spi.c,29 :: 		x = SPIC_Read(0xFF);
	LDI        R27, 255
	MOV        R2, R27
	CALL       _SPIC_Read+0
; x start address is: 17 (R17)
	MOV        R17, R16
	LDI        R18, 0
;spi.c,30 :: 		y = SPIC_Read(0xFF);
	LDI        R27, 255
	MOV        R2, R27
	CALL       _SPIC_Read+0
; y start address is: 19 (R19)
	MOV        R19, R16
	LDI        R20, 0
;spi.c,32 :: 		delay_ms(100);
	PUSH       R18
	PUSH       R17
	LDI        R18, 2
	LDI        R17, 4
	LDI        R16, 187
L_main12:
	DEC        R16
	BRNE       L_main12
	DEC        R17
	BRNE       L_main12
	DEC        R18
	BRNE       L_main12
	NOP
	POP        R17
	POP        R18
;spi.c,34 :: 		UARTC0_Write(x);
	MOV        R2, R17
	CALL       _UARTC0_Write+0
;spi.c,35 :: 		UARTC0_Write(y);
	MOV        R2, R19
	CALL       _UARTC0_Write+0
;spi.c,36 :: 		}
	JMP        L_main8
; x end address is: 17 (R17)
; y end address is: 19 (R19)
;spi.c,37 :: 		}
L_end_main:
	POP        R3
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
