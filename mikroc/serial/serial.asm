
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;serial.c,1 :: 		void main() {
;serial.c,5 :: 		UARTC0_Init(9600);               // Initialize UART module at 9600 bps
	PUSH       R2
	PUSH       R3
	LDS        R16, USARTC0_CTRLB+0
	ANDI       R16, 251
	STS        USARTC0_CTRLB+0, R16
	LDI        R27, 12
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;serial.c,6 :: 		Delay_ms(100);                  // Wait for UART module to stabilize
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
;serial.c,8 :: 		UARTC0_Write_Text("Init");
	LDI        R27, #lo_addr(?lstr1_serial+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr1_serial+0)
	MOV        R3, R27
	CALL       _UARTC0_Write_Text+0
;serial.c,9 :: 		UARTC0_Write(13);
	LDI        R27, 13
	MOV        R2, R27
	CALL       _UARTC0_Write+0
;serial.c,10 :: 		UARTC0_Write(10);
	LDI        R27, 10
	MOV        R2, R27
	CALL       _UARTC0_Write+0
;serial.c,12 :: 		while (1) {                     // Endless loop
L_main2:
;serial.c,13 :: 		if (UARTC0_Data_Ready()) {      // If data is received,
	CALL       _UARTC0_Data_Ready+0
	TST        R16
	BRNE       L__main6
	JMP        L_main4
L__main6:
;serial.c,14 :: 		uart_rd = UARTC0_Read();      // read the received data,
	CALL       _UARTC0_Read+0
;serial.c,15 :: 		UARTC0_Write(uart_rd);        // and send data via UART
	MOV        R2, R16
	CALL       _UARTC0_Write+0
;serial.c,16 :: 		}
L_main4:
;serial.c,17 :: 		}
	JMP        L_main2
;serial.c,18 :: 		}
L_end_main:
	POP        R3
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
