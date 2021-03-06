; Programa que pretende mostrar...
global _funcion, _read_asm, _write_asm

segment .data
caracter1 db 1
caracterlen1 equ $-caracter1
caracter db 20
caracterlen equ $-caracter

segment .text

_funcion:
enter 0,0
;mov eax, [ebp] ; Valor de la dirección de retorno.
;mov eax, [ebp+4] ; Valor del puntero de la pila (original).
mov eax, [ebp+8] ; Primer parámetro.
mov ebx, [ebp+12] ; Segundo parámetro.
add eax, ebx ; Retorno de la suma.
leave
ret

_read_asm:
enter 0,0
;mov eax, [ebp] ; Valor de la dirección de retorno.
;mov eax, [ebp+4] ; Valor del puntero de la pila (original).

mov eax, 3 ; Sys_write
mov ebx, [ebp+8] ; Primer parametro es FileDescriptor
mov ecx, [ebp+12] ; puntero al buffer
mov edx, 0x01 ; largo del buffer (calculado por ensamblador)
int 0x80

mov eax, 0h
mov al, [caracter1] ; Retorno el puntero.
leave
ret

_write_asm:
enter 0,0
;mov eax, [ebp] ; Valor de la dirección de retorno.
;mov eax, [ebp+4] ; Valor del puntero de la pila (original).

mov eax, 4 ; sys_write
mov ebx, [ebp+8] ; Primer parametro = FileDescriptor
mov ecx, [ebp+12] ; Segundo parámetro = Puntero del caracter a escribir
mov edx, 0x01     ; Cantidad de caracteres a escribir.
int 0x80

leave 
ret


imprimir_buffer:
  mov eax, 0x04
  mov ecx, caracter;
  mov edx, 0x10;
  mov ebx, 0x02;
  int 0x80
  ret
