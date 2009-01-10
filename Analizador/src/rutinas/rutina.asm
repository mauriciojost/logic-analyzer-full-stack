; **************** INICIO DE ENCABEZADO. NO TOCARLO...
.486
.model flat, stdcall
option casemap :none
.data
    BUFFER      db 1
.code

; ***************** FIN DE ENCABEZADO.
lee PROC
    mov ah, 02h                 ; Leer el puerto serie
    mov dx, 00h                 ; numero de puerto COM 1
    int 14h                     ; caracter recibido en AL
    RET
lee ENDP

escribe PROC caracter:BYTE
    mov ah, 01h                 ; Escribir el puerto serie
    mov dx, 00h                 ; numero de puerto COM 1
    mov al, caracter            ; Caracter a enviar
    int 14h                     ; devuelve Status en AL
    RET
escribe ENDP
END
