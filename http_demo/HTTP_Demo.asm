
_SPI_Ethernet_UserTCP:
	PUSH       R28
	PUSH       R29
	IN         R28, SPL+0
	IN         R29, SPL+1
	SBIW       R28, 6
	OUT        SPL+0, R28
	OUT        SPL+1, R29
	ADIW       R28, 1

;HTTP_Demo.c,161 :: 		unsigned int  SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags *flags)
; flags start address is: 16 (R16)
	LDD        R16, Y+11
	LDD        R17, Y+12
; flags end address is: 16 (R16)
;HTTP_Demo.c,170 :: 		if(localPort != 80)             // I listen only to web request on port 80
	LDI        R27, 0
	CP         R7, R27
	BRNE       L__SPI_Ethernet_UserTCP17
	LDI        R27, 80
	CP         R6, R27
L__SPI_Ethernet_UserTCP17:
	BRNE       L__SPI_Ethernet_UserTCP18
	JMP        L_SPI_Ethernet_UserTCP0
L__SPI_Ethernet_UserTCP18:
;HTTP_Demo.c,172 :: 		return(0) ;
	LDI        R16, 0
	LDI        R17, 0
	JMP        L_end_SPI_Ethernet_UserTCP
;HTTP_Demo.c,173 :: 		}
L_SPI_Ethernet_UserTCP0:
;HTTP_Demo.c,176 :: 		for(len = 0 ; len < 10 ; len++)
	LDI        R27, 0
	STD        Y+0, R27
	STD        Y+1, R27
L_SPI_Ethernet_UserTCP1:
	LDD        R16, Y+0
	LDD        R17, Y+1
	CPI        R17, 0
	BRNE       L__SPI_Ethernet_UserTCP19
	CPI        R16, 10
L__SPI_Ethernet_UserTCP19:
	BRLO       L__SPI_Ethernet_UserTCP20
	JMP        L_SPI_Ethernet_UserTCP2
L__SPI_Ethernet_UserTCP20:
;HTTP_Demo.c,178 :: 		getRequest[len] = SPI_Ethernet_getByte() ;
	LDI        R18, #lo_addr(_getRequest+0)
	LDI        R19, hi_addr(_getRequest+0)
	LDD        R16, Y+0
	LDD        R17, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+4, R16
	STD        Y+5, R17
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
	LDD        R17, Y+4
	LDD        R18, Y+5
	MOV        R30, R17
	MOV        R31, R18
	ST         Z, R16
;HTTP_Demo.c,176 :: 		for(len = 0 ; len < 10 ; len++)
	LDD        R16, Y+0
	LDD        R17, Y+1
	SUBI       R16, 255
	SBCI       R17, 255
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,179 :: 		}
	JMP        L_SPI_Ethernet_UserTCP1
L_SPI_Ethernet_UserTCP2:
;HTTP_Demo.c,180 :: 		getRequest[len] = 0 ;
	LDI        R18, #lo_addr(_getRequest+0)
	LDI        R19, hi_addr(_getRequest+0)
	LDD        R16, Y+0
	LDD        R17, Y+1
	MOVW       R30, R16
	ADD        R30, R18
	ADC        R31, R19
	LDI        R27, 0
	ST         Z, R27
;HTTP_Demo.c,182 :: 		len = 0;
	LDI        R27, 0
	STD        Y+0, R27
	STD        Y+1, R27
;HTTP_Demo.c,184 :: 		if(memcmp(getRequest, httpMethod, 5))       // only GET method is supported here
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, 5
	MOV        R6, R27
	LDI        R27, 0
	MOV        R7, R27
	LDI        R27, #lo_addr(_httpMethod+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_httpMethod+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_getRequest+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_getRequest+0)
	MOV        R3, R27
	CALL       _memcmp+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	MOV        R27, R16
	OR         R27, R17
	BRNE       L__SPI_Ethernet_UserTCP21
	JMP        L_SPI_Ethernet_UserTCP4
L__SPI_Ethernet_UserTCP21:
;HTTP_Demo.c,186 :: 		return(0) ;
	LDI        R16, 0
	LDI        R17, 0
	JMP        L_end_SPI_Ethernet_UserTCP
;HTTP_Demo.c,187 :: 		}
L_SPI_Ethernet_UserTCP4:
;HTTP_Demo.c,189 :: 		httpCounter++ ;                             // one more request done
	LDS        R16, _httpCounter+0
	LDS        R17, _httpCounter+1
	LDS        R18, _httpCounter+2
	LDS        R19, _httpCounter+3
	SUBI       R16, 255
	SBCI       R17, 255
	SBCI       R18, 255
	SBCI       R19, 255
	STS        _httpCounter+0, R16
	STS        _httpCounter+1, R17
	STS        _httpCounter+2, R18
	STS        _httpCounter+3, R19
;HTTP_Demo.c,191 :: 		if(getRequest[5] == 's')                    // if request path name starts with s, store dynamic data in transmit buffer
	LDS        R16, _getRequest+5
	CPI        R16, 115
	BREQ       L__SPI_Ethernet_UserTCP22
	JMP        L_SPI_Ethernet_UserTCP5
L__SPI_Ethernet_UserTCP22:
;HTTP_Demo.c,196 :: 		len = putConstString(httpHeader) ;              // HTTP header
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_httpHeader+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_httpHeader+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,197 :: 		len += putConstString(httpMimeTypeScript) ;     // with text MIME type
	LDI        R27, #lo_addr(_httpMimeTypeScript+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_httpMimeTypeScript+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,200 :: 		len += putConstString("var PINC=") ;
	LDI        R27, #lo_addr(?lstr_3_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_3_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,201 :: 		WordToStr(PORTC_IN, dyna) ;
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R5, R27
	LDS        R2, PORTC_IN+0
	LDI        R27, 0
	MOV        R3, R27
	CALL       _WordToStr+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
;HTTP_Demo.c,202 :: 		len += putString(dyna) ;
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,203 :: 		len += putConstString(";") ;
	LDI        R27, #lo_addr(?lstr_4_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_4_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,206 :: 		len += putConstString("var PORTD=") ;
	LDI        R27, #lo_addr(?lstr_5_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_5_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,207 :: 		WordToStr(PORTD_OUT, dyna) ;
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R5, R27
	LDS        R2, PORTD_OUT+0
	LDI        R27, 0
	MOV        R3, R27
	CALL       _WordToStr+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
;HTTP_Demo.c,208 :: 		len += putString(dyna) ;
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,209 :: 		len += putConstString(";") ;
	LDI        R27, #lo_addr(?lstr_6_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_6_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,212 :: 		WordToStr(httpCounter, dyna) ;
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R5, R27
	LDS        R2, _httpCounter+0
	LDS        R3, _httpCounter+1
	CALL       _WordToStr+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
;HTTP_Demo.c,213 :: 		len += putConstString("var REQ=") ;
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(?lstr_7_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_7_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,214 :: 		len += putString(dyna) ;
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,215 :: 		len += putConstString(";") ;
	LDI        R27, #lo_addr(?lstr_8_HTTP_Demo+0)
	MOV        R2, R27
	LDI        R27, hi_addr(?lstr_8_HTTP_Demo+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,216 :: 		}
	JMP        L_SPI_Ethernet_UserTCP6
L_SPI_Ethernet_UserTCP5:
;HTTP_Demo.c,217 :: 		else if(getRequest[5] == 't')                           // if request path name starts with t, toggle P3 (LED) bit number that comes after
	LDS        R16, _getRequest+5
	CPI        R16, 116
	BREQ       L__SPI_Ethernet_UserTCP23
	JMP        L_SPI_Ethernet_UserTCP7
L__SPI_Ethernet_UserTCP23:
;HTTP_Demo.c,219 :: 		unsigned char   bitMask = 0 ;                   // for bit mask
;HTTP_Demo.c,221 :: 		if(isdigit(getRequest[6]))                      // if 0 <= bit number <= 9, bits 8 & 9 does not exist but does not matter
	PUSH       R3
	PUSH       R2
	LDS        R2, _getRequest+6
	CALL       _isdigit+0
	POP        R2
	POP        R3
	TST        R16
	BRNE       L__SPI_Ethernet_UserTCP24
	JMP        L_SPI_Ethernet_UserTCP8
L__SPI_Ethernet_UserTCP24:
;HTTP_Demo.c,223 :: 		bitMask = getRequest[6] - '0' ;         // convert ASCII to integer
	LDS        R16, _getRequest+6
	SUBI       R16, 48
;HTTP_Demo.c,224 :: 		bitMask = 1 << bitMask ;                // create bit mask
	MOV        R27, R16
	LDI        R17, 1
	TST        R27
	BREQ       L__SPI_Ethernet_UserTCP26
L__SPI_Ethernet_UserTCP25:
	LSL        R17
	DEC        R27
	BRNE       L__SPI_Ethernet_UserTCP25
L__SPI_Ethernet_UserTCP26:
;HTTP_Demo.c,225 :: 		PORTD_OUT ^= bitMask ;                      // toggle PORTD with xor operator
	LDS        R16, PORTD_OUT+0
	EOR        R16, R17
	STS        PORTD_OUT+0, R16
;HTTP_Demo.c,226 :: 		}
L_SPI_Ethernet_UserTCP8:
;HTTP_Demo.c,227 :: 		}
L_SPI_Ethernet_UserTCP7:
L_SPI_Ethernet_UserTCP6:
;HTTP_Demo.c,229 :: 		if(len == 0)                                            // what do to by default
	LDD        R16, Y+0
	LDD        R17, Y+1
	CPI        R17, 0
	BRNE       L__SPI_Ethernet_UserTCP27
	CPI        R16, 0
L__SPI_Ethernet_UserTCP27:
	BREQ       L__SPI_Ethernet_UserTCP28
	JMP        L_SPI_Ethernet_UserTCP9
L__SPI_Ethernet_UserTCP28:
;HTTP_Demo.c,231 :: 		len =  putConstString(httpHeader) ;             // HTTP header
	PUSH       R9
	PUSH       R8
	PUSH       R7
	PUSH       R6
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_httpHeader+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_httpHeader+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,232 :: 		len += putConstString(httpMimeTypeHTML) ;       // with HTML MIME type
	LDI        R27, #lo_addr(_httpMimeTypeHTML+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_httpMimeTypeHTML+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putConstString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,233 :: 		len += putConstString(indexPage) ;              // HTML page first part
	LDS        R2, _indexPage+0
	LDS        R3, _indexPage+1
	CALL       _SPI_Ethernet_putConstString+0
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,234 :: 		len += putConstString(indexPage2) ;             // HTML page second part
	LDS        R2, _indexPage2+0
	LDS        R3, _indexPage2+1
	CALL       _SPI_Ethernet_putConstString+0
	POP        R2
	POP        R3
	POP        R4
	POP        R5
	POP        R6
	POP        R7
	POP        R8
	POP        R9
	LDD        R18, Y+0
	LDD        R19, Y+1
	ADD        R16, R18
	ADC        R17, R19
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,235 :: 		}
L_SPI_Ethernet_UserTCP9:
;HTTP_Demo.c,237 :: 		return(len) ;                                           // return to the library with the number of bytes to transmit
	LDD        R16, Y+0
	LDD        R17, Y+1
;HTTP_Demo.c,238 :: 		}
L_end_SPI_Ethernet_UserTCP:
	ADIW       R28, 5
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
	SBIW       R28, 2
	OUT        SPL+0, R28
	OUT        SPL+1, R29
	ADIW       R28, 1

;HTTP_Demo.c,250 :: 		unsigned int  SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int destPort, unsigned int reqLength, TEthPktFlags *flags)
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
; flags start address is: 16 (R16)
	LDD        R16, Y+7
	LDD        R17, Y+8
; flags end address is: 16 (R16)
;HTTP_Demo.c,255 :: 		ByteToStr(remoteHost[0], dyna) ;                // first IP address byte
	MOVW       R30, R2
	LD         R16, Z
	PUSH       R5
	PUSH       R4
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R3, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R4, R27
	MOV        R2, R16
	CALL       _ByteToStr+0
	POP        R2
	POP        R3
;HTTP_Demo.c,256 :: 		dyna[3] = '.' ;
	LDI        R27, 46
	STS        _dyna+3, R27
;HTTP_Demo.c,257 :: 		ByteToStr(remoteHost[1], dyna + 4) ;            // second
	MOVW       R30, R2
	ADIW       R30, 1
	LD         R16, Z
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+4)
	MOV        R3, R27
	LDI        R27, hi_addr(_dyna+4)
	MOV        R4, R27
	MOV        R2, R16
	CALL       _ByteToStr+0
	POP        R2
	POP        R3
;HTTP_Demo.c,258 :: 		dyna[7] = '.' ;
	LDI        R27, 46
	STS        _dyna+7, R27
;HTTP_Demo.c,259 :: 		ByteToStr(remoteHost[2], dyna + 8) ;            // third
	MOVW       R30, R2
	ADIW       R30, 2
	LD         R16, Z
	PUSH       R3
	PUSH       R2
	LDI        R27, #lo_addr(_dyna+8)
	MOV        R3, R27
	LDI        R27, hi_addr(_dyna+8)
	MOV        R4, R27
	MOV        R2, R16
	CALL       _ByteToStr+0
	POP        R2
	POP        R3
;HTTP_Demo.c,260 :: 		dyna[11] = '.' ;
	LDI        R27, 46
	STS        _dyna+11, R27
;HTTP_Demo.c,261 :: 		ByteToStr(remoteHost[3], dyna + 12) ;           // fourth
	MOVW       R30, R2
	ADIW       R30, 3
	LD         R16, Z
	LDI        R27, #lo_addr(_dyna+12)
	MOV        R3, R27
	LDI        R27, hi_addr(_dyna+12)
	MOV        R4, R27
	MOV        R2, R16
	CALL       _ByteToStr+0
	POP        R4
	POP        R5
;HTTP_Demo.c,263 :: 		dyna[15] = ':' ;                                // add separator
	LDI        R27, 58
	STS        _dyna+15, R27
;HTTP_Demo.c,266 :: 		WordToStr(remotePort, dyna + 16) ;
	MOVW       R2, R4
	LDI        R27, #lo_addr(_dyna+16)
	MOV        R4, R27
	LDI        R27, hi_addr(_dyna+16)
	MOV        R5, R27
	CALL       _WordToStr+0
;HTTP_Demo.c,267 :: 		dyna[21] = '[' ;
	LDI        R27, 91
	STS        _dyna+21, R27
;HTTP_Demo.c,268 :: 		WordToStr(destPort, dyna + 22) ;
	LDI        R27, #lo_addr(_dyna+22)
	MOV        R4, R27
	LDI        R27, hi_addr(_dyna+22)
	MOV        R5, R27
	MOVW       R2, R6
	CALL       _WordToStr+0
;HTTP_Demo.c,269 :: 		dyna[27] = ']' ;
	LDI        R27, 93
	STS        _dyna+27, R27
;HTTP_Demo.c,270 :: 		dyna[28] = 0 ;
	LDI        R27, 0
	STS        _dyna+28, R27
;HTTP_Demo.c,273 :: 		len = 28 + reqLength;
	MOVW       R16, R8
	SUBI       R16, 228
	SBCI       R17, 255
	STD        Y+0, R16
	STD        Y+1, R17
;HTTP_Demo.c,276 :: 		SPI_Ethernet_putBytes(dyna, 28) ;
	PUSH       R9
	PUSH       R8
	LDI        R27, 28
	MOV        R4, R27
	LDI        R27, 0
	MOV        R5, R27
	LDI        R27, #lo_addr(_dyna+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_dyna+0)
	MOV        R3, R27
	CALL       _SPI_Ethernet_putBytes+0
	POP        R8
	POP        R9
;HTTP_Demo.c,279 :: 		while(reqLength--)
L_SPI_Ethernet_UserUDP10:
	MOVW       R18, R8
	MOVW       R16, R8
	SUBI       R16, 1
	SBCI       R17, 0
	MOVW       R8, R16
	MOV        R27, R18
	OR         R27, R19
	BRNE       L__SPI_Ethernet_UserUDP30
	JMP        L_SPI_Ethernet_UserUDP11
L__SPI_Ethernet_UserUDP30:
;HTTP_Demo.c,281 :: 		SPI_Ethernet_putByte(toupper(SPI_Ethernet_getByte())) ;
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
	PUSH       R3
	PUSH       R2
	MOV        R2, R16
	CALL       _toupper+0
	POP        R2
	POP        R3
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
;HTTP_Demo.c,282 :: 		}
	JMP        L_SPI_Ethernet_UserUDP10
L_SPI_Ethernet_UserUDP11:
;HTTP_Demo.c,284 :: 		return(len) ;           // back to the library with the length of the UDP reply
	LDD        R16, Y+0
	LDD        R17, Y+1
;HTTP_Demo.c,285 :: 		}
;HTTP_Demo.c,284 :: 		return(len) ;           // back to the library with the length of the UDP reply
;HTTP_Demo.c,285 :: 		}
L_end_SPI_Ethernet_UserUDP:
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
; end of _SPI_Ethernet_UserUDP

_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;HTTP_Demo.c,289 :: 		void    main()
;HTTP_Demo.c,291 :: 		OSC_CTRL = 0x02;                 // 32MHz internal RC oscillator
	PUSH       R2
	PUSH       R3
	PUSH       R4
	PUSH       R5
	PUSH       R6
	PUSH       R7
	LDI        R27, 2
	STS        OSC_CTRL+0, R27
;HTTP_Demo.c,293 :: 		while(RC32MRDY_bit == 0)
L_main12:
	LDS        R27, RC32MRDY_bit+0
	SBRC       R27, BitPos(RC32MRDY_bit+0)
	JMP        L_main13
;HTTP_Demo.c,294 :: 		;
	JMP        L_main12
L_main13:
;HTTP_Demo.c,296 :: 		CPU_CCP = 0xD8;
	LDI        R27, 216
	OUT        CPU_CCP+0, R27
;HTTP_Demo.c,297 :: 		CLK_CTRL = 1;
	LDI        R27, 1
	STS        CLK_CTRL+0, R27
;HTTP_Demo.c,310 :: 		PORTC_DIR.B7 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 128
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,311 :: 		PORTC_DIR.B5 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 32
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,312 :: 		PORTC_DIR.B6 = 0;
	LDS        R27, PORTC_DIR+0
	CBR        R27, 64
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,313 :: 		PORTB_DIR.B6 = 0;
	LDS        R27, PORTB_DIR+0
	CBR        R27, 64
	STS        PORTB_DIR+0, R27
;HTTP_Demo.c,314 :: 		PORTB_OUT.B6 = 0;
	LDS        R27, PORTB_OUT+0
	CBR        R27, 64
	STS        PORTB_OUT+0, R27
;HTTP_Demo.c,316 :: 		PORTC_DIR.B0 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 1
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,317 :: 		PORTC_DIR.B1 = 1;
	LDS        R27, PORTC_DIR+0
	SBR        R27, 2
	STS        PORTC_DIR+0, R27
;HTTP_Demo.c,319 :: 		SPIC_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV16, _SPI_CLK_LO_LEADING);
	CLR        R4
	LDI        R27, 1
	MOV        R3, R27
	LDI        R27, 16
	MOV        R2, R27
	CALL       _SPIC_Init_Advanced+0
;HTTP_Demo.c,321 :: 		SPI_Set_Active(&SPIC_Read,&SPIC_Write);
	LDI        R27, #lo_addr(_SPIC_Write+0)
	MOV        R4, R27
	LDI        R27, hi_addr(_SPIC_Write+0)
	MOV        R5, R27
	LDI        R27, #lo_addr(_SPIC_Read+0)
	MOV        R2, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	MOV        R3, R27
	CALL       _SPI_Set_Active+0
;HTTP_Demo.c,322 :: 		Spi_Rd_Ptr = SPIC_Read;
	LDI        R27, #lo_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+0, R27
	LDI        R27, hi_addr(_SPIC_Read+0)
	STS        _SPI_Rd_Ptr+1, R27
;HTTP_Demo.c,323 :: 		SPI_Ethernet_Init(myMacAddr, myIpAddr, Spi_Ethernet_FULLDUPLEX) ;
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
;HTTP_Demo.c,326 :: 		SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr) ;
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
;HTTP_Demo.c,328 :: 		while(1) // do forever
L_main14:
;HTTP_Demo.c,333 :: 		SPI_Ethernet_doPacket() ;   // process incoming Ethernet packets
	CALL       _SPI_Ethernet_doPacket+0
;HTTP_Demo.c,340 :: 		}
	JMP        L_main14
;HTTP_Demo.c,341 :: 		}
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
