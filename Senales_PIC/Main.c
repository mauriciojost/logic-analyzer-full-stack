
#include <16f877a.h> // Pic a utilizar.
#include <stdlib.h>

#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP // Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=12000000) // Clock de 12 MHz.


//#define DELAY delay_ms(100);  // Sólo para probar GUILLE!
#define DELAY;					// Este sirve para simular, y para el original...
//#define DELAY delay_us(30);

int8 actividad=0;


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
		for(j=0;j<7;j++){
			i=i<<1;
			output_d(i);
			DELAY
		}
		for(j=0;j<7;j++){
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

void hacer_cuenta2(){
	int8 i;
	do{
		output_d(i++);
		delay_us(100);
	}while(1);
}

void hacer_cuenta3(){
	int8 i;
	do{
		output_d(i++);
		delay_us(200);
	}while(1);
}

void hacer_aleat(){
	int8 i;
	do{
		i=rand();
		output_d(i);
		
	}while(1);
}

void main(){

	set_tris_d(0x00);

	actividad = (read_eeprom(0x0)+1); 
	actividad = (actividad>8)?0:actividad;

	write_eeprom(0x0,actividad);
	switch(actividad){
		case 0: hacer_cuenta1(); break;
		case 1: hacer_decr1(); break;
		case 2: hacer_parpadeo(); break;
		case 3: hacer_shift(); break;
		case 4: hacer_cuenta2(); break;
		case 5: hacer_cuenta3(); break;
		case 6: hacer_aleat(); break;
		case 7: hacer_cuenta1(); break;
		default: break;
	}

	
}