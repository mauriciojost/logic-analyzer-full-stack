/* 	
	Puesta en funcionamiento
	Se han de hacer las siguientes conexiones:
		PORTD: dedicado al control del LCD (NO UTILIZADO PARA ESTO POR EL MOMENTO)
		C6: transmisión rs232.
		C7: recepción rs232.
	Clock: 20.0 MHz
	En PC:
		Algún programa como Hyperterminal configurado de la siguiente manera: XXX baudios, 8 bits, sin control de flujo.
*/

#include <18f4550.h> // Pic a utilizar.

#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP 					// Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=20000000) 								// Clock de 20 MHz.
//#use rs232 (BAUD=115200,XMIT=PIN_C6,RCV=PIN_C7,BITS=8) 	// Inicialización del módulo rs232.
#use rs232 (BAUD=460800,XMIT=PIN_C6,RCV=PIN_C7,BITS=8) 	// Inicialización del módulo rs232.
#use fast_io(B)											// Lectura rápida de puerto B.

// #include <LCD.C> // Inclusión del driver del lcd. 

#include <stdio.h>
#define FASTER_BUT_MORE_ROM // Para String.h.
#include <string.h>
#include <stdlib.h>
#include "inputm.c"

#define BASE_MUESTRAS 0x400  						// Reserva de memoria RAM para el array datos[1024].
#define QMUESTRAS 	  0x400  						// Base del array en memoria (0x400), y tamaño (0x400 o 1024 elementos).
#reserve BASE_MUESTRAS, BASE_MUESTRAS + QMUESTRAS  	// Reserva propiamente dicha.
int8* datos = BASE_MUESTRAS;

char modo_actual=0; // 0 -> síncrono (clock interno). 1 -> RB(4:7).
unsigned long int periodous_actual=0;
unsigned long int p_datos=0;

void responder_trama();

#include "muestreo.h"
#include "comunicacion.h"
#include "control.h"



void main(){
	inicializacion(); // Inicialización de los elementos del programa.
	/**************** ZONA DE PRUEBAS. ******************/
	/************** FIN DE ZONA DE PRUEBAS. *************/
	rutina_ya_conectado();						// Ante la conexión, se llama a esta rutina.
}