
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

;modbus_experiment.c,83 :: 		unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
; flags start address is: 16 (R16)
	LDD        R16, Y+7
	LDD        R17, Y+8
; flags end address is: 16 (R16)
;modbus_experiment.c,87 :: 		int firstrequest = 0;
;modbus_experiment.c,88 :: 		int requestnumber = 0;
;modbus_experiment.c,95 :: 		if(localPort != 502)             // I listen only to web request on port 80
	LDI        R27, 1
	CP         R7, R27
	BRNE       L__SPI_Ethernet_UserTCP30
	LDI        R27, 246
	CP         R6, R27
L__SPI_Ethernet_UserTCP30:
	BRNE       L__SPI_Ethernet_UserTCP31
	JMP        L_SPI_Ethernet_UserTCP0
L__SPI_Ethernet_UserTCP31:
;modbus_experiment.c,97 :: 		return(0) ;
	LDI        R16, 0
	LDI        R17, 0
	JMP        L_end_SPI_Ethernet_UserTCP
;modbus_experiment.c,98 :: 		}
L_SPI_Ethernet_UserTCP0:
;modbus_experiment.c,100 :: 		PrintOut(PrintHandler, "Recieved %d bytes\r\n", reqLength);
	PUSH       R9
	PUSH       R8
	PUSH       R9
	PUSH       R8
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
	POP        R8
	POP        R9
;modbus_experiment.c,102 :: 		for(i = 0;i < reqLength; i++)
; i start address is: 19 (R19)
	LDI        R19, 0
	LDI        R20, 0
; i end address is: 19 (R19)
L_SPI_Ethernet_UserTCP1:
; i start address is: 19 (R19)
	CP         R19, R8
	CPC        R20, R9
	BRLO       L__SPI_Ethernet_UserTCP32
	JMP        L_SPI_Ethernet_UserTCP2
L__SPI_Ethernet_UserTCP32:
;modbus_experiment.c,105 :: 		((char *)&askframe)[i] = SPI_Ethernet_getByte();
	LDI        R16, #lo_addr(_askframe+0)
	LDI        R17, hi_addr(_askframe+0)
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
;modbus_experiment.c,109 :: 		PrintOut(PrintHandler, "%#02x ", (unsigned int)((char *)&askframe)[i]); // print hex bytes
	LDI        R16, #lo_addr(_askframe+0)
	LDI        R17, hi_addr(_askframe+0)
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
	LDI        R27, hi_addr(?lstr_2_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_2_modbus_experiment+0)
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
;modbus_experiment.c,102 :: 		for(i = 0;i < reqLength; i++)
	MOV        R16, R19
	MOV        R17, R20
	SUBI       R16, 255
	SBCI       R17, 255
	MOV        R19, R16
	MOV        R20, R17
;modbus_experiment.c,111 :: 		}
; i end address is: 19 (R19)
	JMP        L_SPI_Ethernet_UserTCP1
L_SPI_Ethernet_UserTCP2:
;modbus_experiment.c,112 :: 		UART_Write_Text("\r\n");
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(?lstr3_modbus_experiment+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr3_modbus_experiment+0)
	MOV        R3, R27
	CALL       _UART_Write_Text+0
;modbus_experiment.c,114 :: 		PrintOut(PrintHandler, "TS id: %u\r\n", BSWAP_16(askframe.tsid));
	LDS        R20, _askframe+0
	LDS        R21, _askframe+1
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	PUSH       R17
	PUSH       R16
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
	ADIW       R26, 6
	OUT        SPL+0, R26
	OUT        SPL+1, R27
;modbus_experiment.c,115 :: 		PrintOut(PrintHandler, "Protocol id: %u\r\n", BSWAP_16(askframe.protoid));
	LDS        R20, _askframe+2
	LDS        R21, _askframe+3
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_5_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_5_modbus_experiment+0)
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
;modbus_experiment.c,116 :: 		PrintOut(PrintHandler, "Length PDU unit: %u\r\n", BSWAP_16(askframe.length));
	LDS        R20, _askframe+4
	LDS        R21, _askframe+5
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_6_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_6_modbus_experiment+0)
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
;modbus_experiment.c,118 :: 		PrintOut(PrintHandler, "Unit id: %u\r\n", (unsigned int)askframe.pdu.unitid);
	LDS        R16, _askframe+6
	LDI        R17, 0
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_7_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_7_modbus_experiment+0)
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
;modbus_experiment.c,119 :: 		PrintOut(PrintHandler, "Function code: %u\r\n", (unsigned int)askframe.pdu.fncode);
	LDS        R16, _askframe+7
	LDI        R17, 0
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_8_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_8_modbus_experiment+0)
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
;modbus_experiment.c,121 :: 		switch (askframe.pdu.fncode)
	JMP        L_SPI_Ethernet_UserTCP4
;modbus_experiment.c,123 :: 		case 1:
L_SPI_Ethernet_UserTCP6:
;modbus_experiment.c,124 :: 		case 2:
L_SPI_Ethernet_UserTCP7:
;modbus_experiment.c,125 :: 		PrintOut(PrintHandler, "Number of C/D registers requested: %d\r\n", BSWAP_16(askframe.pdu.values.askreadregs.regnumber));
	LDS        R20, _askframe+10
	LDS        R21, _askframe+11
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
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
	LDI        R27, hi_addr(?lstr_9_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_9_modbus_experiment+0)
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
;modbus_experiment.c,126 :: 		askframe.pdu.values.reqreadcoils.bytestofollow = BSWAP_16(askframe.pdu.values.askreadregs.regnumber) / 8;
	LDS        R20, _askframe+10
	LDS        R21, _askframe+11
	MOV        R16, R21
	LDI        R17, 0
	ANDI       R16, 255
	ANDI       R17, 0
	LSR        R17
	ROR        R16
	LSR        R17
	ROR        R16
	LSR        R17
	ROR        R16
	STS        _askframe+8, R16
;modbus_experiment.c,127 :: 		if ((BSWAP_16(askframe.pdu.values.askreadregs.regnumber) % 8)>0)
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	MOVW       R18, R16
	ANDI       R18, 7
	ANDI       R19, 0
	LDI        R16, 0
	LDI        R17, 0
	CP         R16, R18
	CPC        R17, R19
	BRLO       L__SPI_Ethernet_UserTCP33
	JMP        L_SPI_Ethernet_UserTCP8
L__SPI_Ethernet_UserTCP33:
;modbus_experiment.c,128 :: 		askframe.pdu.values.reqreadcoils.bytestofollow++;
	LDS        R16, _askframe+8
	SUBI       R16, 255
	STS        _askframe+8, R16
L_SPI_Ethernet_UserTCP8:
;modbus_experiment.c,129 :: 		askframe.length = BSWAP_16(askframe.pdu.values.reqreadcoils.bytestofollow + 3);
	LDS        R16, _askframe+8
	MOV        R20, R16
	LDI        R21, 0
	SUBI       R20, 253
	SBCI       R21, 255
	MOV        R16, R21
	LDI        R17, 0
	SBRC       R16, 7
	LDI        R17, 255
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	STS        _askframe+4, R16
	STS        _askframe+5, R17
;modbus_experiment.c,131 :: 		for (i = 0; i < askframe.pdu.values.reqreadcoils.bytestofollow; i++)
; i start address is: 18 (R18)
	LDI        R18, 0
	LDI        R19, 0
; i end address is: 18 (R18)
L_SPI_Ethernet_UserTCP9:
; i start address is: 18 (R18)
	LDS        R16, _askframe+8
	CP         R18, R16
	LDI        R27, 0
	CPC        R19, R27
	BRLT       L__SPI_Ethernet_UserTCP34
	JMP        L_SPI_Ethernet_UserTCP10
L__SPI_Ethernet_UserTCP34:
;modbus_experiment.c,132 :: 		askframe.pdu.values.reqreadcoils.coils[i] = 0x00;
	LDI        R16, #lo_addr(_askframe+9)
	LDI        R17, hi_addr(_askframe+9)
	MOVW       R30, R18
	ADD        R30, R16
	ADC        R31, R17
	LDI        R27, 0
	ST         Z, R27
;modbus_experiment.c,131 :: 		for (i = 0; i < askframe.pdu.values.reqreadcoils.bytestofollow; i++)
	MOVW       R16, R18
	SUBI       R16, 255
	SBCI       R17, 255
	MOVW       R18, R16
;modbus_experiment.c,132 :: 		askframe.pdu.values.reqreadcoils.coils[i] = 0x00;
; i end address is: 18 (R18)
	JMP        L_SPI_Ethernet_UserTCP9
L_SPI_Ethernet_UserTCP10:
;modbus_experiment.c,133 :: 		break;
	JMP        L_SPI_Ethernet_UserTCP5
;modbus_experiment.c,134 :: 		case 3:
L_SPI_Ethernet_UserTCP12:
;modbus_experiment.c,135 :: 		case 4:
L_SPI_Ethernet_UserTCP13:
;modbus_experiment.c,136 :: 		firstrequest = BSWAP_16(askframe.pdu.values.askreadregs.firstreg);
	LDS        R20, _askframe+8
	LDS        R21, _askframe+9
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
; firstrequest start address is: 24 (R24)
	MOVW       R24, R16
;modbus_experiment.c,137 :: 		PrintOut(PrintHandler, "Requesing register starting from:: %d\r\n", firstrequest);
	PUSH       R25
	PUSH       R24
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
	LDI        R27, hi_addr(?lstr_10_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_10_modbus_experiment+0)
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
	POP        R24
	POP        R25
;modbus_experiment.c,138 :: 		requestnumber = BSWAP_16(askframe.pdu.values.askreadregs.regnumber);
	LDS        R20, _askframe+10
	LDS        R21, _askframe+11
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
; requestnumber start address is: 22 (R22)
	MOVW       R22, R16
;modbus_experiment.c,139 :: 		PrintOut(PrintHandler, "Numer of H/I registers requested: %d\r\n", requestnumber);
	PUSH       R23
	PUSH       R22
	PUSH       R25
	PUSH       R24
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
	LDI        R27, hi_addr(?lstr_11_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_11_modbus_experiment+0)
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
	POP        R24
	POP        R25
	POP        R22
	POP        R23
;modbus_experiment.c,140 :: 		askframe.pdu.values.reqreadholdings.bytestofollow = requestnumber * 2;
	MOV        R16, R22
	LSL        R16
	STS        _askframe+8, R16
;modbus_experiment.c,141 :: 		askframe.length = BSWAP_16(askframe.pdu.values.reqreadholdings.bytestofollow + 3);
	MOV        R20, R16
	LDI        R21, 0
	SUBI       R20, 253
	SBCI       R21, 255
	MOV        R16, R21
	LDI        R17, 0
	SBRC       R16, 7
	LDI        R17, 255
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOV        R16, R20
	LDI        R17, 0
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	STS        _askframe+4, R16
	STS        _askframe+5, R17
;modbus_experiment.c,143 :: 		for (i = 0; i < requestnumber;i++)
; i start address is: 16 (R16)
	LDI        R16, 0
	LDI        R17, 0
; firstrequest end address is: 24 (R24)
; requestnumber end address is: 22 (R22)
; i end address is: 16 (R16)
	MOVW       R12, R24
	MOVW       R24, R16
	MOVW       R10, R22
L_SPI_Ethernet_UserTCP14:
; i start address is: 24 (R24)
; firstrequest start address is: 12 (R12)
; requestnumber start address is: 10 (R10)
; firstrequest start address is: 12 (R12)
; firstrequest end address is: 12 (R12)
	CP         R24, R10
	CPC        R25, R11
	BRLT       L__SPI_Ethernet_UserTCP35
	JMP        L_SPI_Ethernet_UserTCP15
L__SPI_Ethernet_UserTCP35:
; firstrequest end address is: 12 (R12)
;modbus_experiment.c,145 :: 		if(firstrequest+i < amount) // if requested register within allocated range
; firstrequest start address is: 12 (R12)
	MOVW       R18, R24
	ADD        R18, R12
	ADC        R19, R13
	LDS        R16, _amount+0
	LDS        R17, _amount+1
	CP         R18, R16
	CPC        R19, R17
	BRLO       L__SPI_Ethernet_UserTCP36
	JMP        L_SPI_Ethernet_UserTCP17
L__SPI_Ethernet_UserTCP36:
;modbus_experiment.c,146 :: 		askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(table[firstrequest+i]);
	MOVW       R18, R24
	LSL        R18
	ROL        R19
	LDI        R16, #lo_addr(_askframe+9)
	LDI        R17, hi_addr(_askframe+9)
	MOVW       R22, R18
	ADD        R22, R16
	ADC        R23, R17
	MOVW       R16, R24
	ADD        R16, R12
	ADC        R17, R13
	MOVW       R18, R16
	LSL        R18
	ROL        R19
	LDI        R16, #lo_addr(_table+0)
	LDI        R17, hi_addr(_table+0)
	MOVW       R30, R18
	ADD        R30, R16
	ADC        R31, R17
	LD         R20, Z+
	LD         R21, Z+
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	MOVW       R30, R22
	ST         Z+, R16
	ST         Z+, R17
	JMP        L_SPI_Ethernet_UserTCP18
L_SPI_Ethernet_UserTCP17:
;modbus_experiment.c,148 :: 		askframe.pdu.values.reqreadholdings.registers[i] = BSWAP_16(0x0000); // fill up with zeroes
	MOVW       R18, R24
	LSL        R18
	ROL        R19
	LDI        R16, #lo_addr(_askframe+9)
	LDI        R17, hi_addr(_askframe+9)
	MOVW       R30, R18
	ADD        R30, R16
	ADC        R31, R17
	LDI        R27, 0
	ST         Z+, R27
	ST         Z+, R27
L_SPI_Ethernet_UserTCP18:
;modbus_experiment.c,143 :: 		for (i = 0; i < requestnumber;i++)
	MOVW       R16, R24
	SUBI       R16, 255
	SBCI       R17, 255
	MOVW       R24, R16
;modbus_experiment.c,149 :: 		}
; requestnumber end address is: 10 (R10)
; firstrequest end address is: 12 (R12)
; i end address is: 24 (R24)
	JMP        L_SPI_Ethernet_UserTCP14
L_SPI_Ethernet_UserTCP15:
;modbus_experiment.c,150 :: 		break;
	JMP        L_SPI_Ethernet_UserTCP5
;modbus_experiment.c,151 :: 		case 5:
L_SPI_Ethernet_UserTCP19:
;modbus_experiment.c,152 :: 		case 6:
L_SPI_Ethernet_UserTCP20:
;modbus_experiment.c,154 :: 		break;
	JMP        L_SPI_Ethernet_UserTCP5
;modbus_experiment.c,155 :: 		case 15:
L_SPI_Ethernet_UserTCP21:
;modbus_experiment.c,156 :: 		case 16:
L_SPI_Ethernet_UserTCP22:
;modbus_experiment.c,157 :: 		askframe.length = BSWAP_16(6);
	LDI        R27, 0
	STS        _askframe+4, R27
	LDI        R27, 6
	STS        _askframe+5, R27
;modbus_experiment.c,158 :: 		default:
L_SPI_Ethernet_UserTCP23:
;modbus_experiment.c,159 :: 		break;
	JMP        L_SPI_Ethernet_UserTCP5
;modbus_experiment.c,160 :: 		}
L_SPI_Ethernet_UserTCP4:
	LDS        R16, _askframe+7
	CPI        R16, 1
	BRNE       L__SPI_Ethernet_UserTCP37
	JMP        L_SPI_Ethernet_UserTCP6
L__SPI_Ethernet_UserTCP37:
	LDS        R16, _askframe+7
	CPI        R16, 2
	BRNE       L__SPI_Ethernet_UserTCP38
	JMP        L_SPI_Ethernet_UserTCP7
L__SPI_Ethernet_UserTCP38:
	LDS        R16, _askframe+7
	CPI        R16, 3
	BRNE       L__SPI_Ethernet_UserTCP39
	JMP        L_SPI_Ethernet_UserTCP12
L__SPI_Ethernet_UserTCP39:
	LDS        R16, _askframe+7
	CPI        R16, 4
	BRNE       L__SPI_Ethernet_UserTCP40
	JMP        L_SPI_Ethernet_UserTCP13
L__SPI_Ethernet_UserTCP40:
	LDS        R16, _askframe+7
	CPI        R16, 5
	BRNE       L__SPI_Ethernet_UserTCP41
	JMP        L_SPI_Ethernet_UserTCP19
L__SPI_Ethernet_UserTCP41:
	LDS        R16, _askframe+7
	CPI        R16, 6
	BRNE       L__SPI_Ethernet_UserTCP42
	JMP        L_SPI_Ethernet_UserTCP20
L__SPI_Ethernet_UserTCP42:
	LDS        R16, _askframe+7
	CPI        R16, 15
	BRNE       L__SPI_Ethernet_UserTCP43
	JMP        L_SPI_Ethernet_UserTCP21
L__SPI_Ethernet_UserTCP43:
	LDS        R16, _askframe+7
	CPI        R16, 16
	BRNE       L__SPI_Ethernet_UserTCP44
	JMP        L_SPI_Ethernet_UserTCP22
L__SPI_Ethernet_UserTCP44:
	JMP        L_SPI_Ethernet_UserTCP23
L_SPI_Ethernet_UserTCP5:
;modbus_experiment.c,162 :: 		len = BSWAP_16(askframe.length) + 6;
	LDS        R20, _askframe+4
	LDS        R21, _askframe+5
	MOV        R16, R21
	LDI        R17, 0
	MOVW       R18, R16
	ANDI       R18, 255
	ANDI       R19, 0
	MOVW       R16, R20
	ANDI       R16, 255
	ANDI       R17, 0
	MOV        R17, R16
	CLR        R16
	OR         R16, R18
	OR         R17, R19
	SUBI       R16, 250
	SBCI       R17, 255
; len start address is: 18 (R18)
	MOVW       R18, R16
;modbus_experiment.c,163 :: 		PrintOut(PrintHandler, "Reply length: %d\r\n", len);
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
	PUSH       R17
	PUSH       R16
	LDI        R27, hi_addr(?lstr_12_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_12_modbus_experiment+0)
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
	POP        R18
	POP        R19
;modbus_experiment.c,165 :: 		SPI_Ethernet_putBytes((unsigned char *)&askframe,len);
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
	MOVW       R4, R18
	LDI        R27, #lo_addr(_askframe+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_askframe+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putBytes+0
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
;modbus_experiment.c,168 :: 		return(len);
	MOVW       R16, R18
; len end address is: 18 (R18)
;modbus_experiment.c,170 :: 		}
;modbus_experiment.c,168 :: 		return(len);
;modbus_experiment.c,170 :: 		}
L_end_SPI_Ethernet_UserTCP:
	POP        R5
	POP        R4
	POP        R3
	POP        R2
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

;modbus_experiment.c,172 :: 		unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
; flags start address is: 16 (R16)
	LDD        R16, Y+0
	LDD        R17, Y+1
; flags end address is: 16 (R16)
;modbus_experiment.c,174 :: 		return(0);
	LDI        R16, 0
	LDI        R17, 0
;modbus_experiment.c,175 :: 		}
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

;modbus_experiment.c,180 :: 		void    main()
;modbus_experiment.c,182 :: 		OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
	PUSH       R6
	PUSH       R7
	LDI        R27, 2
	STS        OSC_CTRL+0, R27
;modbus_experiment.c,183 :: 		while(RC32MRDY_bit == 0)
L_main24:
	LDS        R27, RC32MRDY_bit+0
	SBRC       R27, BitPos(RC32MRDY_bit+0)
	JMP        L_main25
;modbus_experiment.c,184 :: 		;
	JMP        L_main24
L_main25:
;modbus_experiment.c,185 :: 		CPU_CCP = 0xD8;
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;modbus_experiment.c,186 :: 		CLK_CTRL = 1;
	LDI        R27, 1
	STS        CLK_CTRL+0, R27
;modbus_experiment.c,195 :: 		PORTC_DIR.B7 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 128
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,196 :: 		PORTC_DIR.B5 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 32
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,197 :: 		PORTC_DIR.B6 = 0;
	LDS        R27, PORTC_DIR+0
	CBR        R27, 64
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,198 :: 		PORTB_DIR.B6 = 0;
	LDS        R27, PORTB_DIR+0
	CBR        R27, 64
	STS        PORTB_DIR+0, R27
;modbus_experiment.c,199 :: 		PORTB_OUT.B6 = 0;
	LDS        R27, PORTB_OUT+0
	CBR        R27, 64
	STS        PORTB_OUT+0, R27
;modbus_experiment.c,200 :: 		PORTC_DIR.B0 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 1
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,201 :: 		PORTC_DIR.B1 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 2
	STS        PORTC_DIR+0, R27
;modbus_experiment.c,203 :: 		UARTC0_Init(115200);
	LDS        R27, USARTC0_CTRLB+0
	SBR        R27, 4
	STS        USARTC0_CTRLB+0, R27
	LDI        R27, 34
	STS        USARTC0_BAUDCTRLA+0, R27
	LDI        R27, 0
	STS        USARTC0_BAUDCTRLB+0, R27
	CALL       _UARTC0_Init+0
;modbus_experiment.c,204 :: 		UART_Set_Active(&UARTC0_Read, &UARTC0_Write, &UARTC0_Data_Ready, &UARTC0_Tx_Idle);
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
;modbus_experiment.c,206 :: 		UART_Write_Text("Starting\r\n");
	LDI        R27, #lo_addr(?lstr13_modbus_experiment+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr13_modbus_experiment+0)
	MOV        R3, R27
	CALL       _UART_Write_Text+0
;modbus_experiment.c,207 :: 		PrintOut(PrintHandler, "Testing output\r\n");
	LDI        R27, hi_addr(?lstr_14_modbus_experiment+0)
	PUSH       R27
	LDI        R27, #lo_addr(?lstr_14_modbus_experiment+0)
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
;modbus_experiment.c,209 :: 		SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
	CLR        R4
	LDI        R27, 1
	MOV        R3, R27
	LDI        R27, 16
	MOV        R2, R27
	CALL       _SPIC_Init_Advanced+0
;modbus_experiment.c,210 :: 		SPI_Set_Active(&SPIC_Read,&SPIC_Write);
	LDI        R27, #lo_addr(_SPIC_Write+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_SPIC_Write+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_SPIC_Read+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	MOV        R3, R27
	CALL       _SPI_Set_Active+0
;modbus_experiment.c,211 :: 		Spi_Rd_Ptr = SPIC_Read;
	LDI        R27, #lo_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+0, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+1, R27
;modbus_experiment.c,212 :: 		SPI_Ethernet_Init(myMacAddr, myIpAddr, Spi_Ethernet_FULLDUPLEX) ;
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
;modbus_experiment.c,215 :: 		SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;
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
;modbus_experiment.c,217 :: 		while(1) // do forever
L_main26:
;modbus_experiment.c,222 :: 		SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
	CALL       _SPI_Ethernet_doPacket+0
;modbus_experiment.c,229 :: 		}
	JMP        L_main26
;modbus_experiment.c,230 :: 		}
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
