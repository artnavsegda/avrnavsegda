
_main:
	LDI        R27, 255
	OUT        SPL+0, R27
	LDI        R27, 0
	OUT        SPL+1, R27

;i2c.c,1 :: 		void main()
;i2c.c,3 :: 		TWIE_Init(100000);
	PUSH       R2
	LDI        R27, 5
	STS        TWIE_MASTER_BAUD+0, R27
	CALL       _TWIE_Init+0
;i2c.c,4 :: 		TWIE_Start();
	CALL       _TWIE_Start+0
;i2c.c,5 :: 		TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
	LDI        R27, 52
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,6 :: 		TWIE_Write(0x03);             // send byte (address of location)
	LDI        R27, 3
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,7 :: 		TWIE_Write(0x00);          // send data (data to be written)
	CLR        R2
	CALL       _TWIE_Write+0
;i2c.c,8 :: 		TWIE_Stop();               // issue TWI stop signal
	CALL       _TWIE_Stop+0
;i2c.c,10 :: 		Delay_100ms();
	CALL       _Delay_100ms+0
;i2c.c,12 :: 		TWIE_Init(100000);
	LDI        R27, 5
	STS        TWIE_MASTER_BAUD+0, R27
	CALL       _TWIE_Init+0
;i2c.c,13 :: 		TWIE_Start();
	CALL       _TWIE_Start+0
;i2c.c,14 :: 		TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
	LDI        R27, 52
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,15 :: 		TWIE_Write(0x02);             // send byte (address of location)
	LDI        R27, 2
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,16 :: 		TWIE_Write(0x00);          // send data (data to be written)
	CLR        R2
	CALL       _TWIE_Write+0
;i2c.c,17 :: 		TWIE_Stop();               // issue TWI stop signal
	CALL       _TWIE_Stop+0
;i2c.c,19 :: 		while (1)
L_main0:
;i2c.c,21 :: 		Delay_100ms();
	CALL       _Delay_100ms+0
;i2c.c,23 :: 		TWIE_Init(100000);
	LDI        R27, 5
	STS        TWIE_MASTER_BAUD+0, R27
	CALL       _TWIE_Init+0
;i2c.c,24 :: 		TWIE_Start();
	CALL       _TWIE_Start+0
;i2c.c,25 :: 		TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
	LDI        R27, 52
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,26 :: 		TWIE_Write(0x01);             // send byte (address of location)
	LDI        R27, 1
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,27 :: 		TWIE_Write(0x00);          // send data (data to be written)
	CLR        R2
	CALL       _TWIE_Write+0
;i2c.c,28 :: 		TWIE_Stop();               // issue TWI stop signal
	CALL       _TWIE_Stop+0
;i2c.c,30 :: 		Delay_100ms();
	CALL       _Delay_100ms+0
;i2c.c,32 :: 		TWIE_Init(100000);
	LDI        R27, 5
	STS        TWIE_MASTER_BAUD+0, R27
	CALL       _TWIE_Init+0
;i2c.c,33 :: 		TWIE_Start();
	CALL       _TWIE_Start+0
;i2c.c,34 :: 		TWIE_Write(0x1a << 1);          // send byte via TWI (device address + W)
	LDI        R27, 52
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,35 :: 		TWIE_Write(0x01);             // send byte (address of location)
	LDI        R27, 1
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,36 :: 		TWIE_Write(0xFF);          // send data (data to be written)
	LDI        R27, 255
	MOV        R2, R27
	CALL       _TWIE_Write+0
;i2c.c,37 :: 		TWIE_Stop();               // issue TWI stop signal
	CALL       _TWIE_Stop+0
;i2c.c,38 :: 		}
	JMP        L_main0
;i2c.c,39 :: 		}
L_end_main:
	POP        R2
L__main_end_loop:
	JMP        L__main_end_loop
; end of _main
