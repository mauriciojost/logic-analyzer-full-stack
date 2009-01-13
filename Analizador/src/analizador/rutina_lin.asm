; Programa que pretende mostrar...
global _funcion
segment .data
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

