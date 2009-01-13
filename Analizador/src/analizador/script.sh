#!/bin/sh
echo "Compilando Comunicador.java..."
echo "javac Comunicador.java"
cd ..
echo "Generando el .h..."
echo "javah -jni analizador.Comunicador"
echo "mv *.h ./analizador/"
cd analizador
echo "Compilando el .asm..."
echo "ml rutina.asm /c /coff"
echo "Compilando el .c..."
gcc Comunicado.c -ocomunicado.so -Wl,-soname,libnative.so -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/linux -shared
echo "Copiando .so al directorio misdoc..."
cp comunicado.so /usr/lib/comunicado.so
cd ../../dist
echo "Ejecutando el .jar..."
java -jar Analizador.jar
echo "Volviendo a src..."
cd ..
cd src
cd analizador
