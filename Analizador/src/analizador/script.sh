#!/bin/sh
echo "Compilando Comunicador.java..."
javac Comunicador.java
cd ..
echo "Generando el .h..."
javah -jni analizador.Comunicador
mv *.h ./analizador/
cd analizador
echo "Compilando el .asm..."
echo "nasm rutina.asm /c /coff"
echo "Compilando el .c..."
gcc Incomunicado.c -ocomunicado.so -Wl,-soname,libnative.so -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/linux -shared
echo "Copiando .so al directorio /usr/lib/..."
cp comunicado.so /usr/lib/comunicado.so
echo "Ejecutando el .jar..."
echo "java -jar ../../dist/Analizador.jar"
echo "Volviendo a src..."

