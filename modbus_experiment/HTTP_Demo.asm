
_PrintHandler:

;HTTP_Demo.c,29 :: 		void PrintHandler(char c)
;HTTP_Demo.c,31 :: 		UART_Write(c);
	CALL       _UART_Write+0
;HTTP_Demo.c,32 :: 		}
L_end_PrintHandler:
	RET
; end of _PrintHandler

_SPI_Ethernet_UserTCP:
	PUSH       R28
	PUSH       R29
	IN         R28, SPL+0
	IN         R29, SPL+1
	ADIW       R28, 6

;HTTP_Demo.c,37 :: 		unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
	PUSH       R6
	PUSH       R7
; flags start address is: 16 (R16)
	LDD        R16, Y+0
	LDD        R17, Y+1
; flags end address is: 16 (R16)
;HTTP_Demo.c,47 :: 		if(localPort != 502)             // I listen only to web request on port 80
	LDI        R27, 1
	CP         R7, R27
	BRNE       L__SPI_Ethernet_UserTCP10
	LDI        R27, 246
	CP         R6, R27
L__SPI_Ethernet_UserTCP10:
	BRNE       L__SPI_Ethernet_UserTCP11
	JMP        L_SPI_Ethernet_UserTCP0
L__SPI_Ethernet_UserTCP11:
;HTTP_Demo.c,49 :: 		return(0) ;
	LDI        R16, 0
	LDI        R17, 0
	JMP        L_end_SPI_Ethernet_UserTCP
;HTTP_Demo.c,50 :: 		}
L_SPI_Ethernet_UserTCP0:
;HTTP_Demo.c,52 :: 		SPI_Ethernet_getBytes(buf, ERDPT, reqLength);
	PUSH       R9
	PUSH       R8
	MOVW       R6, R8
	CLR        R4
	CLR        R5
	LDI        R27, #lo_addr(_buf+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_buf+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_getBytes+0
	POP        R8
	POP        R9
;HTTP_Demo.c,54 :: 		for(i = 0;i < reqLength; i++)
; i start address is: 18 (R18)
	LDI        R18, 0
	LDI        R19, 0
; i end address is: 18 (R18)
L_SPI_Ethernet_UserTCP1:
; i start address is: 18 (R18)
	CP         R18, R8
	CPC        R19, R9
	BRLO       L__SPI_Ethernet_UserTCP12
	JMP        L_SPI_Ethernet_UserTCP2
L__SPI_Ethernet_UserTCP12:
;HTTP_Demo.c,57 :: 		PrintOut(PrintHandler, "%#2X ");
	PUSH       R19
	PUSH       R18
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, hi_addr(?lstr_1_HTTP_Demo+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_1_HTTP_Demo+0)
	PUSH       R27
	LDI        R27, hi_addr(_PrintHandler+0)
	PUSH       R27
	LDI        R27, #lo_addr(_PrintHandler+0)
	PUSH       R27
	CALL       _PrintOut+0
	IN         R26, SPL+0
	IN         R27, SPL+1
	ADIW       R26, 4
	OUT        SPL+0, R26
	OUT        SPL+1, R27
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	POP        R18
	POP        R19
;HTTP_Demo.c,54 :: 		for(i = 0;i < reqLength; i++)
	MOVW       R16, R18
	SUBI       R16, 255
	SBCI       R17, 255
	MOVW       R18, R16
;HTTP_Demo.c,58 :: 		}
; i end address is: 18 (R18)
	JMP        L_SPI_Ethernet_UserTCP1
L_SPI_Ethernet_UserTCP2:
;HTTP_Demo.c,60 :: 		UART_Write_Text("\n\r");
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(?lstr2_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr2_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _UART_Write_Text+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
;HTTP_Demo.c,62 :: 		return(0);
	LDI        R16, 0
	LDI        R17, 0
;HTTP_Demo.c,63 :: 		}
;HTTP_Demo.c,62 :: 		return(0);
;HTTP_Demo.c,63 :: 		}
L_end_SPI_Ethernet_UserTCP:
	POP        R7
	POP        R6
	POP        R5
	POP        R4
	POP        R3
	POP        R2
	POP        R29
	POP        R28
	RET
; end of _SPI_Ethernet_UserTCP

_SPI_Ethernet_UserUDP:
	PUSH       R28
	PUSH       R29
	IN         R28, SPL+0
	IN         R29, SPL+1
	ADIW       R28, 6

;HTTP_Demo.c,65 :: 		unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
; flags start address is: 16 (R16)
	LDD        R16, Y+0
	LDD        R17, Y+1
; flags end address is: 16 (R16)
;HTTP_Demo.c,67 :: 		return(0);
	LDI        R16, 0
	LDI        R17, 0
;HTTP_Demo.c,68 :: 		}
L_end_SPI_Ethernet_UserUDP:
	POP        R29
	POP        R28
	RET
; end of _SPI_Ethernet_UserUDP

_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;HTTP_Demo.c,73 :: 		void    main()
;HTTP_Demo.c,75 :: 		OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
	PUSH       R6
	PUSH       R7
	LDI        R27, 2
	STS        OSC_CTRL+0, R27
;HTTP_Demo.c,76 :: 		while(RC32MRDY_bit == 0)
L_main4:
	LDS        R27, RC32MRDY_bit+0
	SBRC       R27, BitPos(RC32MRDY_bit+0)
	JMP        L_main5
;HTTP_Demo.c,77 :: 		;
	JMP        L_main4
L_main5:
;HTTP_Demo.c,78 :: 		CPU_CCP = 0xD8;
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;HTTP_Demo.c,79 :: 		CLK_CTRL = 1;
	LDI        R27, 1
	STS        CLK_CTRL+0, R27
;HTTP_Demo.c,88 :: 		PORTC_DIR.B7 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 128
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,89 :: 		PORTC_DIR.B5 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 32
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,90 :: 		PORTC_DIR.B6 = 0;
	LDS        R27, PORTC_DIR+0
	CBR        R27, 64
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,91 :: 		PORTB_DIR.B6 = 0;
	LDS        R27, PORTB_DIR+0
	CBR        R27, 64
	STS        PORTB_DIR+0, R27
;HTTP_Demo.c,92 :: 		PORTB_OUT.B6 = 0;
	LDS        R27, PORTB_OUT+0
	CBR        R27, 64
	STS        PORTB_OUT+0, R27
;HTTP_Demo.c,93 :: 		PORTC_DIR.B0 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 1
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,94 :: 		PORTC_DIR.B1 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 2
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,96 :: 		UARTC0_Init(115200);
	LDS        R27, USARTC0_CTRLB+0
	SBR        R27, 4
	STS        USARTC0_CTRLB+0, R27
	LDI        R27, 34
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;HTTP_Demo.c,97 :: 		UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
	LDI        R27, #lo_addr(_UARTC0_Tx_Idle+0)
	MOV        R6, R27
	LDI        R27, hi_addr(_UARTC0_Tx_Idle+0)
	MOV        R7, R27
	LDI        R27, #lo_addr(_UARTC0_Write+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_UARTC0_Write+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_UARTC0_Read+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_UARTC0_Read+0)
	MOV        R3, R27
	LDI        R27, hi_addr(_UARTC0_Data_Ready+0)
	PUSH       R27
	LDI        R27, #lo_addr(_UARTC0_Data_Ready+0)
	PUSH       R27
	CALL       _UART_Set_Active+0
	IN         R26, SPL+0
	IN         R27, SPL+1
	ADIW       R26, 2
	OUT        SPL+0, R26
	OUT        SPL+1, R27
;HTTP_Demo.c,99 :: 		UART_Write_Text("Starting\n\r");
	LDI        R27, #lo_addr(?lstr3_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr3_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _UART_Write_Text+0
;HTTP_Demo.c,101 :: 		SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
	CLR        R4
	LDI        R27, 1
	MOV        R3, R27
	LDI        R27, 16
	MOV        R2, R27
	CALL       _SPIC_Init_Advanced+0
;HTTP_Demo.c,102 :: 		SPI_Set_Active(&SPIC_Read,&SPIC_Write);
	LDI        R27, #lo_addr(_SPIC_Write+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_SPIC_Write+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_SPIC_Read+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	MOV        R3, R27
	CALL       _SPI_Set_Active+0
;HTTP_Demo.c,103 :: 		Spi_Rd_Ptr = SPIC_Read;
	LDI        R27, #lo_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+0, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+1, R27
;HTTP_Demo.c,104 :: 		SPI_Ethernet_Init(myMacAddr, myIpAddr, Spi_Ethernet_FULLDUPLEX) ;
	LDI        R27, 1
	MOV        R6, R27
	LDI        R27, #lo_addr(_myIpAddr+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_myIpAddr+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_myMacAddr+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_myMacAddr+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_Init+0
;HTTP_Demo.c,107 :: 		SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;
	LDI        R27, #lo_addr(_dnsIpAddr+0)
	MOV        R6, R27
	LDI        R27, hi_addr(_dnsIpAddr+0)
	MOV        R7, R27
	LDI        R27, #lo_addr(_gwIpAddr+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_gwIpAddr+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_ipMask+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_ipMask+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_confNetwork+0
;HTTP_Demo.c,109 :: 		while(1) // do forever
L_main6:
;HTTP_Demo.c,114 :: 		SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
	CALL       _SPI_Ethernet_doPacket+0
;HTTP_Demo.c,121 :: 		}
	JMP        L_main6
;HTTP_Demo.c,122 :: 		}
L_end_main:
	POP        R7
	POP        R6
	POP        R5
	POP        R4
	POP        R3
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
