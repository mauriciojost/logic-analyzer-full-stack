#!/bin/bash

echo "Ejecutar en modo root!..."
mkdir /dev/usb
mknod /dev/usb/ttyUSB0 c 188 0
mknod /dev/usb/ttyUSB1 c 188 1
mknod /dev/usb/ttyUSB2 c 188 2

chmod 777 -R /dev/usb/
echo "Puerto serie, listo."
