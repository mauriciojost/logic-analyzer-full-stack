#define POSTINC0 0x0FEE
#define FSR0     0x0000
#define PORTB    0x0F81
#define FSR0L    0x0FE9
#define FSR0H    0x0FEA


void borrar_buffer_muestras(){
	long i;
	for(i=0;i<1024;i++)
		datos[i] = 0x00;
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
}

// Función que realiza el muestreo.
void muestrear500KHz(){
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
}

// Función que realiza el muestreo.
void muestrear_periodico(){
	long int k;
	disable_interrupts(GLOBAL);
	for(k=0;k<QMUESTRAS;k++){
		datos[k]=input_b(); // original
		delay_us(periodous_actual);
	}
}

void iniciar_muestreo_sincrono(){
	set_tris_b(0xFF);
	disable_interrupts(GLOBAL);
	if (periodous_actual>6){
		muestrear_periodico();
	}else if(periodous_actual>1){
		periodous_actual = 2;
		muestrear500kHz();	
	}else{
		periodous_actual = 1;
		muestrear1MHz();
	}
	responder_trama();
	disable_interrupts(GLOBAL);
}



// Función que trata a la interrupción producida por un cambio en RB.
#INT_RB
void interrupcion_rb(){
	datos[p_datos++]=input_b();
	clear_interrupt(INT_RB);	
}

void iniciar_muestreo_asincrono(){
	set_tris_b(0xFF);
	
	p_datos=0;
	enable_interrupts(GLOBAL); // Configuración de las interrupciones.
	enable_interrupts(INT_RB); // Configuración de las interrupciones.
	disable_interrupts(INT_EXT); // Configuración de las interrupciones.

	while(p_datos<QMUESTRAS){}

	disable_interrupts(INT_RB); // Configuración de las interrupciones.	
	responder_trama();
}

void iniciar_muestreo(){
	switch(modo_actual){
		case 0: iniciar_muestreo_sincrono(); 
				break;
		case 1: iniciar_muestreo_asincrono(); 
				break;
	}
}