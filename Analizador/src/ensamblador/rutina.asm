;Para compilar hacer:
; ml.exe Addup.asm /c /coff

.486
.model flat, stdcall
option casemap :none

.code

Addup PROC, Arg1:DWORD, Arg2:DWORD, Arg3:DWORD

mov eax, Arg1
add eax, Arg2
add eax, Arg3

ret

Addup ENDP
;----------------------------------------------

END
;----------------------------------------------