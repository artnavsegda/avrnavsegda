
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;serial.c,1 :: 		void main() {
;serial.c,5 :: 		OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
	PUSH       R2
	PUSH       R3
	LDI        R27, 2
	STS        OSC_CTRL+0, R27
;serial.c,6 :: 		while(RC32MRDY_bit == 0);
L_main0:
	LDS        R27, RC32MRDY_bit+0
	SBRC       R27, BitPos(RC32MRDY_bit+0)
	JMP        L_main1
	JMP        L_main0
L_main1:
;serial.c,7 :: 		CPU_CCP = 0xD8;
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;serial.c,8 :: 		CLK_CTRL = 1;
	LDI        R27, 1
	STS        CLK_CTRL+0, R27
;serial.c,10 :: 		UARTC0_Init(9600);               // Initialize UART module at 9600 bps
	LDS        R27, USARTC0_CTRLB+0
	SBR        R27, 4
	STS        USARTC0_CTRLB+0, R27
	LDI        R27, 160
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 1
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;serial.c,11 :: 		Delay_ms(100);                  // Wait for UART module to stabilize
	LDI        R18, 17
	LDI        R17, 60
	LDI        R16, 204
L_main2:
	DEC        R16
	BRNE       L_main2
	DEC        R17
	BRNE       L_main2
	DEC        R18
	BRNE       L_main2
;serial.c,13 :: 		UARTC0_Write_Text("Hello world from here!");
	LDI        R27, #lo_addr(?lstr1_serial+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr1_serial+0)
	MOV        R3, R27
	CALL       _UARTC0_Write_Text+0
;serial.c,17 :: 		while (1) {                     // Endless loop
L_main4:
;serial.c,18 :: 		if (UARTC0_Data_Ready()) {      // If data is received,
	CALL       _UARTC0_Data_Ready+0
	TST        R16
	BRNE       L__main8
	JMP        L_main6
L__main8:
;serial.c,19 :: 		uart_rd = UARTC0_Read();      // read the received data,
	CALL       _UARTC0_Read+0
;serial.c,20 :: 		UARTC0_Write(uart_rd);        // and send data via UART
	MOV        R2, R16
	CALL       _UARTC0_Write+0
;serial.c,21 :: 		}
L_main6:
;serial.c,22 :: 		}
	JMP        L_main4
;serial.c,23 :: 		}
L_end_main:
	POP        R3
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
