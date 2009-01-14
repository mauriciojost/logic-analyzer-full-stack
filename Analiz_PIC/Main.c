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

/*
1.Inicialización.
2.Envío de 'listo'.
	a.Espera de comando.
	b.Inicia rutina del comando recibido.
	c.Vuelve al punto 2.a.
*/

#define POSTINC0 0x0FEE
#define FSR0     0x0000
#define PORTB    0x0F81
#define FSR0L    0x0FE9
#define FSR0H    0x0FEA

#include <18f4550.h> // Pic a utilizar.

#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP 					// Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=20000000) 								// Clock de 20 MHz.
#use rs232 (BAUD=115200,XMIT=PIN_C6,RCV=PIN_C7,BITS=8) 	// Inicialización del módulo rs232.
#use fast_io(B)											// Lectura rápida de puerto B.

// #include <LCD.C> // Inclusión del driver del lcd. 

#define FASTER_BUT_MORE_ROM // Para String.h.
#include <STDLIB.H>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE_MUESTRAS 0x400  						// Reserva de memoria RAM para el array datos[1024].
#define QMUESTRAS 	  0x400  						// Base del array en memoria (0x400), y tamaño (0x400 o 1024 elementos).
#reserve BASE_MUESTRAS, BASE_MUESTRAS + QMUESTRAS  	// Reserva propiamente dicha.

int8 auxiliar;
int8* datos = BASE_MUESTRAS;

char modo_actual=0; // 0 es el por defecto (clock interno). 1 es por cambio de nivel de RB(4:7).
unsigned long int periodous_actual=0;
unsigned long int i=0;


// Función que trata a la interrupción producida por un flanco en la pata INT.
#INT_EXT 
void interrupcion_int(){
	printf("\n\rInterrupcion int..."); // El caracter es impreso en ambos medios.
	clear_interrupt(INT_EXT);	
}


// Retorna true si es valido, o 0 si es no valido.
int parseo(char* orden){
	
	char pat_inicio[] = "<inicio";
	char pat_final[] = "> </inicio>";
	char pat_nuevo[] = "nuevo=";
	char pat_modo[] = "modo=";
	char pat_velocidad[] = "velocidad=";
	
	char nuevo[6], modo[6], velocidad[6];
	int k;
	unsigned long cantidad;
	
	int nuevo_int;
	
	char* p_inicio, *p_final, *p_nuevo, *p_modo, *p_velocidad;
	
	//printf("Parseo... Orden:'%s' (en %d).\n\r",orden,(int)orden);

	p_inicio    = strstr(orden, pat_inicio);  // Al vicio.
	p_nuevo     = strstr(orden, pat_nuevo);	
	p_final     = strstr(orden, pat_final);
	p_modo      = strstr(orden, pat_modo);
	p_velocidad = strstr(orden, pat_velocidad);

	//printf("\n\rPunteros p_inicio=%lu p_nuevo=%lu p_final=%lu p_modo=%lu p_veloc=%lu.\n\r",(long)p_inicio, (long)p_nuevo, (long)p_final, (long)p_modo, (long)p_velocidad);	

	if (((long)p_inicio==0)||((long)p_nuevo==0)||((long)p_final==0)||((long)p_modo==0)||((long)p_velocidad==0)){
		return 0;
	}

	for(k=0;k<6;k++){
		nuevo[k]    = 0;
		modo[k]     = 0;
		velocidad[k]= 0;
	}

	cantidad = (unsigned long)(p_modo  - (p_nuevo+6+1));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(nuevo     , p_nuevo+6     , cantidad);
	cantidad = (int)(p_velocidad  - (p_modo+5+1));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(modo      , p_modo+5      , cantidad);
	cantidad = (int)(p_final - (p_velocidad+10));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(velocidad , p_velocidad+10, cantidad);
	

	//printf("\n\rCadenas nuevo='%s' modo='%s' veloc='%s'.\n\r",nuevo, modo, velocidad);
	
	

	nuevo_int = atoi(nuevo);
	modo_actual = atoi(modo);
	periodous_actual = atol(velocidad);

	//printf("\n\rValores nuevo=%d modo=%d veloc=%lu.\n\r", nuevo_int, modo_actual, periodous_actual);
	return 1;
}


// Función que tiene por fin realizar la inicialización de los elementos del programa general.
void inicializacion(){
	//lcd_init(); // Inicialización del módulo de LCD.

	//set_tris_b(0xFF); // original
	set_tris_d(0xFF);  /// OBOOOBOORRARRAARRRR SE USA PARA EL LCD!!!

	disable_interrupts(INT_EXT); // Configuración de las interrupciones.
	disable_interrupts(INT_RB); // Configuración de las interrupciones.
	enable_interrupts(GLOBAL);
}

int8 get_crc(){
	int8 sum = 0;
	for(i=0;i<QMUESTRAS;i++){
		sum += datos[i];
	}
	return sum;
}


// Función que realiza una representación de las muestras realizadas.
void mostrar_muestras(){
	
	printf("<inicio nuevo=1 modo=%d velocidad=%lu> ",modo_actual,periodous_actual);
	for(i=0;i<QMUESTRAS;i++){
		//printf("%x ",datos[i]);
		printf("%u ",(unsigned)datos[i]);
	}
	printf("<CRC> %u </CRC> </inicio>\n",(unsigned)get_crc());
}




// Función que realiza el muestreo.
void muestrear1MHz(){
	disable_interrupts(GLOBAL);

	
	#asm
		LFSR FSR0, BASE_MUESTRAS
		
	BUCLE1M:
		MOVFF PORTB, POSTINC0
		BTFSS FSR0H, 3
		BRA BUCLE1M
	#endasm 
	
	//delay_us(periodous_actual);
	
	mostrar_muestras();
	disable_interrupts(GLOBAL);	
}



// Función que realiza el muestreo.
void muestrear500KHz(){
	disable_interrupts(GLOBAL);

	
	#asm
		LFSR FSR0, BASE_MUESTRAS
		
	BUCLE500:
		MOVFF PORTB, POSTINC0
		NOP
		NOP
		NOP
		NOP
		NOP
		BTFSS FSR0H, 3
		BRA BUCLE500
	#endasm 
	
	delay_us(periodous_actual);
	
	mostrar_muestras();
	disable_interrupts(GLOBAL);	
}



// Función que realiza el muestreo.
void muestrear(){
	disable_interrupts(GLOBAL);

	for(i=0;i<QMUESTRAS;i++){
		datos[i]=input_b(); // original
		delay_us(periodous_actual);
	}
	mostrar_muestras();
	disable_interrupts(GLOBAL);
}



void muestrear_rb(){
	set_tris_b(0xFF);
	
	i=0;
	enable_interrupts(GLOBAL); // Configuración de las interrupciones.
	enable_interrupts(INT_RB); // Configuración de las interrupciones.
	disable_interrupts(INT_EXT); // Configuración de las interrupciones.

	while(i<QMUESTRAS){}

	disable_interrupts(INT_RB); // Configuración de las interrupciones.	
	mostrar_muestras();
}

// Función que trata a la interrupción producida por un cambio en RB.
#INT_RB
void interrupcion_rb(){
	datos[i++]=input_b();
	clear_interrupt(INT_RB);	
}




void modo_conf(){
	//printf("\n\r        Modo (0 periodico, 1 asincrono)? %u-> ",(unsigned int)modo); // Mensaje en pantalla de PC.
	//modo = get_int(); // Obtención del valor ingresado.
	//if (modo_actual==0){
	//	printf("\n\r        Periodo? %luuS-> ",periodous); // Mensaje en pantalla de PC.
		//periodous = get_long();
	//	printf("\n\r        Hecho! (modo periódico, periodo %luuS).", periodous); // Mensaje en pantalla de PC.
	//}else{
	//	printf("\n\r        Hecho! (modo asincronico)."); // Mensaje en pantalla de PC.
	//}
	
}

void borrar(){
	long i;
	for(i=0;i<1024;i++)
		datos[i] = 0x00;
}
void decidir_rutina_muestreo(){
	borrar();
	set_tris_b(0xFF);
	if      (periodous_actual>6){
		muestrear();
	}else if(periodous_actual>1){
		periodous_actual = 2;
		muestrear500kHz();	
	}else{
		periodous_actual = 1;
		muestrear1MHz();
	}

}

void rutina_ya_conectado(){
	char orden[100];
	char tecla;
	unsigned int j=0;

	do{
		
		tecla = getc(); // Obtención del caracter presionado.

		// Tecla de inicio del muestreo.
		if (tecla == 13){ // Cambiar por /N!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!dfsgasd!!!!!!!!1
			j=0;
			if (parseo(orden)){
			
				if (modo_actual==0){
					decidir_rutina_muestreo();
				}
				if (modo_actual==1){
					muestrear_rb();
				}
			}
		}else{
			orden[j++]=tecla;
			orden[j]=0;
			//printf("Orden: '%s' (%d).\n\r",orden,(int)tecla);
		}


	}while(tecla!='y');
}




void main(){
	inicializacion(); // Inicialización de los elementos del programa.
	/**************** ZONA DE PRUEBAS. ******************/


	


	/************** FIN DE ZONA DE PRUEBAS. *************/
	rutina_ya_conectado();						// Ante la conexión, se llama a esta rutina.
}