
#include <16f877a.h> // Pic a utilizar.

#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP // Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=12000000) // Clock de 12 MHz.


//#define DELAY delay_ms(100);  // Sólo para probar GUILLE!
#define DELAY;					// Este sirve para simular, y para el original...

int8 i=0;





void hacer_parpadeo(){
	int8 i=0;
	do{
		i = ~i;
		output_d(i);
		DELAY
	}while(1);
}

void hacer_shift(){
	int8 i=1;
	int8 j;
	do{
		for(j=0;j<8;j++){
			i=i<<1;
			output_d(i);
			DELAY
		}
		for(j=0;j<8;j++){
			i=i>>1;
			output_d(i);
			DELAY
		}

	}while(1);
}



void hacer_decr1(){
	int8 i;
	do{
		output_d(i--);
		DELAY
	}while(1);
}

void hacer_cuenta1(){
	int8 i;
	do{
		output_d(i++);
		DELAY
	}while(1);
}



// Función que trata a la interrupción producida por un flanco en la pata INT.
#INT_EXT
void interrupcion_int(){
	
	clear_interrupt(INT_EXT);
	i++;
	switch(i){
		case 0: hacer_cuenta1(); break;
		case 1: hacer_decr1(); break;
		case 2: hacer_cuenta1(); break;
		case 3: hacer_cuenta1(); break;
		case 4: hacer_cuenta1(); break;
	}
}




void main(){
	set_tris_d(0x00);
	enable_interrupts(INT_EXT); // Configuración de las interrupciones.
	enable_interrupts(GLOBAL);	
	while(1){}
}