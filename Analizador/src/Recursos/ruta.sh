#!/bin/bash
ruta=$(locate include/jni.h)
JDK=${ruta%/jni.h}
echo "Ruta de JDK encontrada:"
echo $JDK
echo ""
