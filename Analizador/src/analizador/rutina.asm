
.486
.model flat, stdcall
option casemap :none

.data

PUERTO_BASE DW 03f8h ; Se corresponde com COM1

.code

inicia PROC

	; El siguiente proceso configura el puerto COM1
	; con los siguientes parametros: xxxx bps,N,8,1
	; Activacion de de bit DLAB.

	MOV DX,PUERTO_BASE
	ADD DX,3 ; Se posiciona en RCL (el que tiene a DLAB, que sirve para poder establecer la velocidad de la comunicaci�n).
	MOV AL,128 ; Selecciona el bit 7 de RCL
	OUT DX,AL ; Activa bit 7 del registro de control de linea (DLAB).
	; Carga word del registro divisor (9600).
	; Para 9600 bps esta debe tener cargado un 0x000C (12 decimal).
	MOV DX,PUERTO_BASE ; Se posiciona en byte menos significativo.
	MOV AL,1 ; de reg. divisor (12 corresponde a 9600bps, 1 a 115200 bps)
	OUT DX,AL ; Carga byte menos significativo de reg. divisor.
	; Carga byte mas significativo del registro divisor (9600)
	MOV DX,PUERTO_BASE ;
	ADD DX,1 ; Se posiciona en byte m�s significativo
	MOV AL,0 ; de reg. divisor (en +1 respecto de por ejemplo 0x3F8 para COM1).
	OUT DX,AL ; Carga byte m�s significativo de reg. divisor.
	; Desactivacion de DLAB
	MOV DX,PUERTO_BASE
	ADD DX,3 ; Se posiciona en RCL
	MOV AL,0 ; Selecciona el bit 7 de RCL
	OUT DX,AL ; Desactiva bit 7 del registro de control de linea
	MOV AL,3  ; Para RCL...
	OUT DX,AL ;Configura RCL con 8,N,1 (DLAB,BREAK,PARITY(3),LENGTH_STOP_BIP,WORD_LENGTH(2))

	; Se desactivan todas las interrupciones de la UART del puerto COM1 (3F8H)
	MOV DX,PUERTO_BASE
	ADD DX,1 ; Se posiciona en reg. activador de interrupciones
	MOV AL,0 ; Desactiva todas las interrupciones
	OUT DX,AL
	RET

inicia ENDP



lee PROC
	; COMPRUEBA PUERTO
	MOV DX,PUERTO_BASE
	ADD DX,5 ; Se posiciona en reg. de estado de linea
	IN AL,DX ; Lee registro LSR
	TEST AL,00000001B ; Detecta que hay un caracter esperando a
	; ser leido en la UART.
	JNZ RECEPCION ;
        RET
	;TEST AL,01000000B ; Pregunta por bit 6 (reg. de trans. vacio)
	;JNZ TRANSMISION
RECEPCION:
	; Lee caracter del registo buffer de recepcion RBR
	MOV DX,PUERTO_BASE ; Se posiciona en RBR
	IN AL,DX ; Lee el registro
        MOV AH, AL
        MOV BL, AL
        MOV AL, AL
        MOV CL, AL
        MOV DX, 0
        MOV BX, 0
        
        
	RET

lee ENDP


escribe PROC, Arg1:BYTE
	MOV AL, Arg1
	MOV DX,PUERTO_BASE ; Se posiciona en reg. de retencion de transmision
	OUT DX,AL ; se envia el contenido de AL al THR
	RET
escribe ENDP



END
;----------------------------------------------