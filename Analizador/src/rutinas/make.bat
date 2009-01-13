@echo off
del main.exe
ml.exe rutina.asm /c /coff
cl main.cpp rutina.obj
del *.obj
main.exe

cmd