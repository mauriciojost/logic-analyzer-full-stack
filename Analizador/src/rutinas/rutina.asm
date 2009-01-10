; **************** INICIO DE ENCABEZADO. NO TOCARLO...
.486
.model flat, stdcall
option casemap :none
.data
    BUFFER      db 1
.code

; ***************** FIN DE ENCABEZADO.
lee PROC, fd:WORD
    mov ah, 3fh                 ; Leer el archivo o dispositivo
    mov bx, fd                  ; usar el manejar del archivo
    mov cx, 1                   ; leer de a un caracter
    mov edx, OFFSET BUFFER       ; Lugar donde se almacena el caracter leido (DS:DX)
    int 21h                     ;
;   jc FIN_LECTURA              ; saltar si hay error
;   or ax, ax                   ; saltar si se llegó al final del archivo
;   jz FIN_LECTURA
;FIN_LECTURA:
    mov al, BUFFER
    RET
lee ENDP

escribe PROC, fd:WORD, caracter:BYTE
    mov bx, fd                  ; escribimos la información leída
    mov cx, 1                   ; escribir de a un caracter
    mov al, caracter
		mov BUFFER, al
    mov edx, OFFSET BUFFER      ; Lugar donde se almacena el caracter a escribir (DS:DX)
    mov ah, 40h
    int 21h
;    jc FIN_ESCRITURA            ; saltar si hay error
;    or ax, ax                   ; saltar si se llegó al final del archivo
;    jz FIN_ESCRITURA
;FIN_ESCRITURA:
    mov al, BUFFER
    RET
escribe ENDP
END
