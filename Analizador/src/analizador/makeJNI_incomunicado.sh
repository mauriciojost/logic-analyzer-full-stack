#!/bin/bash/usr/bin/javac /home/mauri/misdoc/javaw/analiz/trunk/Analizador/src/analizador/Comunicador.java
cd ..
echo Generando el .h...
javah -jni analizador.Comunicador
mv *.h .\analizador\
cd analizador
echo Compilando el .asm...
echo ml rutina.asm /c /coff
echo Compilando el .c...
cl -I"%JDK%\include\win32" -I"%JDK%\include" -LD Incomunicado.cpp 
echo rutina.obj
echo Incluir al final rutina.obj. Está excluida de la compilacion del C por ahora.
cp incomunicado.dll c:\windows\system32\Comunicado.dll

