#!/bin/sh
cd ../analizador/
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
gcc Comunicado.c -ocomunicado.so -Wl,-soname,libnative.so -I$JDK -I$JDK/linux -shared
echo "Copiando .so al directorio /usr/lib/..."
cp comunicado.so /usr/lib/comunicado.so
echo "Borrando los archivos generados..."
rm *.class
rm *.h
rm *.so
echo "Listo."
