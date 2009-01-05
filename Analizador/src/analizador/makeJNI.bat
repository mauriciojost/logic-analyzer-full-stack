@echo off
javac Comunicador.java
cd ..
javah -jni analizador.Comunicador
move *.h .\analizador\
cd analizador
echo ml rutina.asm /c /coff
cl -I"C:\Archivos de programa\Java\jdk1.6.0_01\include\win32" -I"C:\Archivos de programa\Java\jdk1.6.0_01\include" -LD Comunicado.cpp 
echo rutina.obj
copy Comunicado.dll c:\windows\system32\
del *.class
del *.obj
del *.dll
del *.exp
del *.lib
pause