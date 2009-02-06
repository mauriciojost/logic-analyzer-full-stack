/**** 16F877 ****/
/*
#include <16f877a.h> // Pic a utilizar.
#include <stdlib.h>
#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP // Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=12000000) // Clock de 12 MHz.
//#define DELAY delay_ms(100);  // Sólo para probar GUILLE!
#define DELAY;					// Este sirve para simular, y para el original...
//#define DELAY delay_us(30);
*/

/**** 16f84A ****/

#include <16f84A.h> // Pic a utilizar.
#include <stdlib.h>
#fuses XT,NOWDT,NOPROTECT,PUT // Fuses, tipo de oscilador, WDT off, sin protección de código...
#use delay(clock=4000000) // Clock de 4 MHz.
#define DELAY;





int8 actividad=0;
void hacer_parpadeo(){
	int8 i=0;
	do{
		i = ~i;
		output_b(i);
		DELAY
	}while(1);
}

void hacer_autof(){
	int8 i=1;
	int8 j;
	do{
		for(j=0;j<7;j++){
			i=i<<1;
			output_b(i);
			DELAY
		}
		for(j=0;j<7;j++){
			i=i>>1;
			output_b(i);
			DELAY
		}
	}while(1);
}

void hacer_shift(){
	int8 i=1;
	int8 j;
	do{
		for(j=0;j<8;j++){
			output_b(i);	
			i=i<<1;
			DELAY
		}
		i=1;
	}while(1);
}


void hacer_shift1(){
	int8 i=1;
	int8 j;
	do{
		for(j=0;j<7;j++){
			
			output_b(i);	
			i=i<<1;
			DELAY
			delay_us(20);
		}
		i=1;
	}while(1);
}


void hacer_decr1(){
	int8 i;
	do{
		output_b(i--);
		DELAY
	}while(1);
}

void hacer_cuenta1(){
	int8 i;
	do{
		output_b(i++);
		DELAY
	}while(1);
}

void hacer_cuenta2(){
	int8 i;
	do{
		output_b(i++);
		delay_us(100);
	}while(1);
}

void hacer_cuenta3(){
	int8 i;
	do{
		output_b(i++);
		delay_us(500);
	}while(1);
}

void hacer_aleat(){
	int8 i;
	do{
		i=rand();
		output_b(i);
	}while(1);
}

void hacer_const(){
	int8 i=0xAA;
	do{
		output_b(i);
	}while(1);
}

void main(){

	set_tris_b(0x00);
	delay_ms(1000);

	actividad = (read_eeprom(0x0)+1); 
	actividad = (actividad>9)?0:actividad;

	write_eeprom(0x0,actividad);
	switch(actividad){
		case 0: hacer_cuenta1(); break;
		case 1: hacer_decr1(); break;
		case 2: hacer_parpadeo(); break;
		case 3: hacer_shift(); break;
		case 4: hacer_cuenta2(); break;
		case 5: hacer_cuenta3(); break;
		case 6: hacer_aleat(); break;
		case 7: hacer_autof(); break;
		case 8: hacer_shift1(); break;
		default: hacer_const(); break;
	}

	
}