#!/bin/sh
echo "Ejecutar como root..."
echo "*********** CONFIGURANDO PUERTO SERIE..."
./serie_USB.sh
echo "*********** CONFIGURANDO ELEMENTOS DE LA APLICACION..."
./make_comunicado.sh
echo "*********** EJECUTANDO..."
java -jar ../../dist/Analizador.jar
