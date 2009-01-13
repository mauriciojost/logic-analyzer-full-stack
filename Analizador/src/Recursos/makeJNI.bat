@echo off
echo Recuerde definir la variable de entorno JDK.
echo Esta puede ser por ejemplo:
echo    C:\Archivos de programa\Java\jdk1.6.0_01
echo 
echo Compilando el .java...
javac Comunicador.java
cd ..
echo Generando el .h...
javah -jni analizador.Comunicador
move *.h .\analizador\
cd analizador
echo Compilando el .asm...
echo ml rutina.asm /c /coff
echo Compilando el .c...
cl -I"%JDK%\include\win32" -I"%JDK%\include" -LD Comunicado.cpp 
echo rutina.obj
echo Incluir al final rutina.obj. Está excluida de la compilacion del C por ahora.
copy Comunicado.dll c:\windows\system32\
@echo off
del *.class
del *.obj
del *.dll
del *.exp
del *.lib
pause