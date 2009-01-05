echo @echo off
echo Recuerde definir la variable de entorno JDK.
echo Esta puede ser por ejemplo:
echo    C:\Archivos de programa\Java\jdk1.6.0_01
javac Comunicador.java
cd ..
javah -jni analizador.Comunicador
move *.h .\analizador\
cd analizador
echo ml rutina.asm /c /coff
cl -I"%JDK%\include\win32" -I"%JDK%\include" -LD Comunicado.cpp 
echo rutina.obj
copy Comunicado.dll c:\windows\system32\
del *.class
del *.obj
del *.dll
del *.exp
del *.lib
pause