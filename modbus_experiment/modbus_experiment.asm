
_PrintHandler:

;modbus_experiment.c,29 :: 		void PrintHandler(char c)
;modbus_experiment.c,31 :: 		UART_Write(c);
	CALL       _UART_Write+0
;modbus_experiment.c,32 :: 		}
L_end_PrintHandler:
	RET
; end of _PrintHandler

_SPI_Ethernet_UserTCP:
	PUSH       R28
	PUSH       R29
	IN         R28, SPL+0
	IN         R29, SPL+1
	SBIW       R28, 2
	OUT        SPL+0, R28
	OUT        SPL+1, R29
	ADIW       R28, 1

;modbus_experiment.c,37 :: 		unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
; flags start address is: 16 (R16)
	LDD        R16, Y+7
	LDD        R17, Y+8
; flags end address is: 16 (R16)
;modbus_experiment.c,47 :: 		if(localPort != 502)             // I listen only to web request on port 80
	LDI        R27, 1
	CP         R7, R27
	BRNE       L__SPI_Ethernet_UserTCP10
	LDI        R27, 246
	CP         R6, R27
L__SPI_Ethernet_UserTCP10:
	BRNE       L__SPI_Ethernet_UserTCP11
	JMP        L_SPI_Ethernet_UserTCP0
L__SPI_Ethernet_UserTCP11:
;modbus_experiment.c,49 :: 		return(0) ;
	LDI        R16, 0
	LDI        R17, 0
	JMP        L_end_SPI_Ethernet_UserTCP
;modbus_experiment.c,50 :: 		}
L_SPI_Ethernet_UserTCP0:
;modbus_experiment.c,54 :: 		for(i = 0;i < reqLength; i++)
; i start address is: 19 (R19)
	LDI        R19, 0
	LDI        R20, 0
; i end address is: 19 (R19)
L_SPI_Ethernet_UserTCP1:
; i start address is: 19 (R19)
	CP         R19, R8
	CPC        R20, R9
	BRLO       L__SPI_Ethernet_UserTCP12
	JMP        L_SPI_Ethernet_UserTCP2
L__SPI_Ethernet_UserTCP12:
;modbus_experiment.c,56 :: 		buf[i] = SPI_Ethernet_getByte();
	LDI        R16, #lo_addr(_buf+0)
	LDI        R17, hi_addr(_buf+0)
	ADD        R16, R19
	ADC        R17, R20
	STD        Y+0, R16
	STD        Y+1, R17
	PUSH       R20
	PUSH       R19
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	CALL       _SPI_Ethernet_getByte+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	POP        R19
	POP        R20
	LDD        R17, Y+0
	LDD        R18, Y+1
	MOV        R30, R17
	MOV        R31, R18
	ST         Z, R16
;modbus_experiment.c,57 :: 		Spi_Ethernet_putByte(buf[i]);//send back all recieved modbus packets
	LDI        R16, #lo_addr(_buf+0)
	LDI        R17, hi_addr(_buf+0)
	MOV        R30, R19
	MOV        R31, R20
	ADD        R30, R16
	ADC        R31, R17
	LD         R16, Z
	PUSH       R20
	PUSH       R19
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	MOV        R2, R16
	CALL       _SPI_Ethernet_putByte+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	POP        R19
	POP        R20
;modbus_experiment.c,59 :: 		PrintOut(PrintHandler, "%#02x ", (unsigned int)buf[i]);
	LDI        R16, #lo_addr(_buf+0)
	LDI        R17, hi_addr(_buf+0)
	MOV        R30, R19
	MOV        R31, R20
	ADD        R30, R16
	ADC        R31, R17
	LD         R16, Z
	LDI        R17, 0
	PUSH       R20
	PUSH       R19
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_1_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_1_modbus_experiment+0)
	PUSH       R27
	LDI        R27, hi_addr(_PrintHandler+0)
	PUSH       R27
	LDI        R27, #lo_addr(_PrintHandler+0)
	PUSH       R27
	CALL       _PrintOut+0
	IN         R26, SPL+0
	IN         R27, SPL+1
	ADIW       R26, 6
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
	POP        R19
	POP        R20
;modbus_experiment.c,54 :: 		for(i = 0;i < reqLength; i++)
	MOV        R16, R19
	MOV        R17, R20
	SUBI       R16, 255
	SBCI       R17, 255
	MOV        R19, R16
	MOV        R20, R17
;modbus_experiment.c,61 :: 		}
; i end address is: 19 (R19)
	JMP        L_SPI_Ethernet_UserTCP1
L_SPI_Ethernet_UserTCP2:
;modbus_experiment.c,63 :: 		UART_Write_Text("\r\n");
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(?lstr2_modbus_experiment+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr2_modbus_experiment+0)
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
;modbus_experiment.c,67 :: 		return(reqLength);
	MOVW       R16, R8
;modbus_experiment.c,69 :: 		}
L_end_SPI_Ethernet_UserTCP:
	ADIW       R28, 1
	OUT        SPL+0, R28
	OUT        SPL+1, R29
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

;modbus_experiment.c,71 :: 		unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
; flags start address is: 16 (R16)
	LDD        R16, Y+0
	LDD        R17, Y+1
; flags end address is: 16 (R16)
;modbus_experiment.c,73 :: 		return(0);
	LDI        R16, 0
	LDI        R17, 0
;modbus_experiment.c,74 :: 		}
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

;modbus_experiment.c,79 :: 		void    main()
;modbus_experiment.c,81 :: 		OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
	PUSH       R6
	PUSH       R7
	LDI        R27, 2
	STS        OSC_CTRL+0, R27
;modbus_experiment.c,82 :: 		while(RC32MRDY_bit == 0)
L_main4:
	LDS        R27, RC32MRDY_bit+0
	SBRC       R27, BitPos(RC32MRDY_bit+0)
	JMP        L_main5
;modbus_experiment.c,83 :: 		;
	JMP        L_main4
L_main5:
;modbus_experiment.c,84 :: 		CPU_CCP = 0xD8;
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;modbus_experiment.c,85 :: 		CLK_CTRL = 1;
	LDI        R27, 1
	STS        CLK_CTRL+0, R27
;modbus_experiment.c,94 :: 		PORTC_DIR.B7 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 128
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,95 :: 		PORTC_DIR.B5 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 32
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,96 :: 		PORTC_DIR.B6 = 0;
	LDS        R27, PORTC_DIR+0
	CBR        R27, 64
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,97 :: 		PORTB_DIR.B6 = 0;
	LDS        R27, PORTB_DIR+0
	CBR        R27, 64
	STS        PORTB_DIR+0, R27
;modbus_experiment.c,98 :: 		PORTB_OUT.B6 = 0;
	LDS        R27, PORTB_OUT+0
	CBR        R27, 64
	STS        PORTB_OUT+0, R27
;modbus_experiment.c,99 :: 		PORTC_DIR.B0 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 1
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,100 :: 		PORTC_DIR.B1 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 2
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,102 :: 		UARTC0_Init(115200);
	LDS        R27, USARTC0_CTRLB+0
	SBR        R27, 4
	STS        USARTC0_CTRLB+0, R27
	LDI        R27, 34
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;modbus_experiment.c,103 :: 		UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
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
;modbus_experiment.c,105 :: 		UART_Write_Text("Starting\r\n");
	LDI        R27, #lo_addr(?lstr3_modbus_experiment+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr3_modbus_experiment+0)
	MOV        R3, R27
	CALL       _UART_Write_Text+0
;modbus_experiment.c,106 :: 		PrintOut(PrintHandler, "Testing output\r\n");
	LDI        R27, hi_addr(?lstr_4_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_4_modbus_experiment+0)
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
;modbus_experiment.c,108 :: 		SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
	CLR        R4
	LDI        R27, 1
	MOV        R3, R27
	LDI        R27, 16
	MOV        R2, R27
	CALL       _SPIC_Init_Advanced+0
;modbus_experiment.c,109 :: 		SPI_Set_Active(&SPIC_Read,&SPIC_Write);
	LDI        R27, #lo_addr(_SPIC_Write+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_SPIC_Write+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_SPIC_Read+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	MOV        R3, R27
	CALL       _SPI_Set_Active+0
;modbus_experiment.c,110 :: 		Spi_Rd_Ptr = SPIC_Read;
	LDI        R27, #lo_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+0, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+1, R27
;modbus_experiment.c,111 :: 		SPI_Ethernet_Init(myMacAddr, myIpAddr, Spi_Ethernet_FULLDUPLEX) ;
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
;modbus_experiment.c,114 :: 		SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;
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
;modbus_experiment.c,116 :: 		while(1) // do forever
L_main6:
;modbus_experiment.c,121 :: 		SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
	CALL       _SPI_Ethernet_doPacket+0
;modbus_experiment.c,128 :: 		}
	JMP        L_main6
;modbus_experiment.c,129 :: 		}
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
