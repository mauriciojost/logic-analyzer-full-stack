; **************** INICIO DE ENCABEZADO. NO TOCARLO...
.486
.model flat, stdcall
option casemap :none
.data
    HANDLE_IN   dw 0
    HANDLE_OUT  dw 0
    BUFFER      db 1
.code

; ***************** FIN DE ENCABEZADO.
lee PROC, fd:DWORD
    mov HANDLE_IN, OFFSET fd         ; salvar el manejador de archivo
    mov ah, 3fh                 ; Leer el archivo o dispositivo
    mov bx, HANDLE_IN           ; usar el manejar del archivo
    mov cx, 1                   ; leer de a un caracter
    mov edx, OFFSET BUFFER       ; Lugar donde se almacena el caracter leido (DS:DX)
    int 21h                     ; 

;    jc FIN_LECTURA              ; saltar si hay error

;   or ax, ax                   ; saltar si se llegó al final del archivo
;    jz FIN_LECTURA
;FIN_LECTURA:
    mov al, BUFFER
    RET
lee ENDP

escribe PROC, fd:DWORD, caracter:BYTE
    mov HANDLE_OUT, OFFSET fd        ; guardar el manejador devuelto
    mov bx, HANDLE_OUT          ; escribimos la información leída
    mov cx, 1                   ; escribir de a un caracter
    mov BUFFER, OFFSET caracter
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
