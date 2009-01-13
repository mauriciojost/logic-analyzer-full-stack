#!/bin/sh
echo "Compilando Comunicador.java..."
javac Comunicador.java
cd ..
echo "Generando el .h..."
javah -jni analizador.Comunicador
mv *.h ./analizador/
cd analizador
echo "Compilando el .asm..."
echo "ml rutina.asm /c /coff"
echo "Compilando el .c..."
gcc -o libnativelib.so -shared -Wl,-soname,libnative.so -I//usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/linux Comunicado.c -static -lc
