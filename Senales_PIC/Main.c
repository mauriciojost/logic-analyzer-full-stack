
#include <16f877a.h> // Pic a utilizar.

#fuses HS,NOWDT,NOPROTECT,PUT,NOLVP // Fuses, tipo de oscilador, WDT off, sin protecci�n de c�digo...
#use delay(clock=12000000) // Clock de 12 MHz.
int8 i=0;



void hacer_decr1(){
	int8 i;
	do{
		output_d(i--);
	}while(1);
}

void hacer_cuenta1(){
	int8 i;
	do{
		output_d(i++);
	}while(1);
}



// Funci�n que trata a la interrupci�n producida por un flanco en la pata INT.
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
	enable_interrupts(INT_EXT); // Configuraci�n de las interrupciones.
	enable_interrupts(GLOBAL);	
	while(1){}
}