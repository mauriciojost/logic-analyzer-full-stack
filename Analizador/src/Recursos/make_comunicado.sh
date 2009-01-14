#!/bin/sh
ruta=$(locate include/jni.h)
JDK_ACTUAL=${ruta%/jni.h}
echo "Ruta de JDK encontrada:"
echo $JDK_ACTUAL
cd ../analizador/
echo "Compilando Comunicador.java..."
javac Comunicador.java
cd ..
echo "Generando el .h..."
javah -jni analizador.Comunicador
mv *.h ./analizador/
cd analizador
echo "Compilando el .asm..."
echo "nasm rutina_lin.asm -felf"
echo "Compilando el .c..."
gcc Comunicado.c -ocomunicado.so -Wl,-soname,libnative.so -I$JDK_ACTUAL -I$JDK_ACTUAL/linux -shared
echo "Copiando .so al directorio /usr/lib/..."
cp comunicado.so /usr/lib/comunicado.so
echo "Borrando los archivos generados..."
rm *.class
rm *.h
rm *.so
rm *.o
echo "Listo."
