#define CARACTER_FIN_TRAMA '\r'
#define CARACTER_FLUSH '\n'

#define RCSTA 0x0FAB
#define CREN 0x0004


// Retorna true si es valido, o 0 si es no valido.
int parseo(char* orden){
	char pat_inicio[] = "<i";
	char pat_final[] = "> </i>";
	char pat_nuevo[] = "n=";
	char pat_modo[] = "m=";
	char pat_velocidad[] = "p=";

	char nuevo[6], modo[6], velocidad[6];
	unsigned long cantidad;
	int k;
	char* p_inicio, *p_final, *p_nuevo, *p_modo, *p_velocidad;
	
	//printf("Parseo... Orden:'%s' (en %d).\n\r",orden,(int)orden);

	p_inicio    = strstr(orden, pat_inicio);
	p_nuevo     = strstr(orden, pat_nuevo);	
	p_final     = strstr(orden, pat_final);
	p_modo      = strstr(orden, pat_modo);
	p_velocidad = strstr(orden, pat_velocidad);

	//printf("\n\rPunteros p_inicio=%lu p_nuevo=%lu p_final=%lu p_modo=%lu p_veloc=%lu.\n\r",(long)p_inicio, (long)p_nuevo, (long)p_final, (long)p_modo, (long)p_velocidad);	

	if (((long)p_inicio==0)||((long)p_nuevo==0)||((long)p_final==0)||((long)p_modo==0)||((long)p_velocidad==0)){ // Cada una de las partes debe estar presente.
		return 0;
	}

	for(k=0;k<6;k++){ nuevo[k]    = 0; modo[k]     = 0; velocidad[k]= 0; }

	// <inicio nuevo=1 modo=1 velocidad=100> </inicio>
	// Busqueda de nuevo. Entr
	cantidad = (unsigned long)(p_modo  - (p_nuevo+strlen(pat_nuevo)+1));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(nuevo     , p_nuevo+strlen(pat_nuevo)     , cantidad);
	
	// Busqueda de modo.
	cantidad = (int)(p_velocidad  - (p_modo+strlen(pat_modo)+1));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(modo      , p_modo+strlen(pat_modo)      , cantidad);

	// Busqueda de periodo.
	cantidad = (int)(p_final - (p_velocidad+strlen(pat_velocidad)));
	//printf("Cantidad: '%lu'.\n\r",cantidad);
	strncpy(velocidad , p_velocidad+strlen(pat_velocidad), cantidad);

	//printf("\n\rCadenas nuevo='%s' modo='%s' veloc='%s'.\n\r",nuevo, modo, velocidad);

	nuevo_actual = atoi(nuevo);
	modo_actual = atoi(modo);
	periodous_actual = atol(velocidad);

	//printf("\n\rValores nuevo=%d modo=%d veloc=%lu.\n\r", nuevo_int, modo_actual, periodous_actual);
	return 1;
}

int8 get_crc(){
	int8 sum = 0;
	unsigned long int k;
	for(k=0;k<QMUESTRAS;k++){
		sum += datos[k];
	}
	return sum;
}


// Función que realiza una representación de las muestras realizadas.
void responder_trama(){
	unsigned long int k;
	lcd_transmitiendo();

	if (nuevo_actual==1){ /* Tradicional. */
		printf("<inicio nuevo=1 modo=%d velocidad=%lu> ",modo_actual,periodous_actual);
		for(k=0;k<QMUESTRAS;k++){
			printf("%u ",(unsigned)datos[k]);
		}
		printf("<CRC> %u </CRC> </inicio>\n",(unsigned)get_crc());
	}

	if (nuevo_actual==2){ /* Largo, en hexa. */
		printf("<inicio nuevo=1 modo=%d velocidad=%lu> ",modo_actual,periodous_actual);
		for(k=0;k<QMUESTRAS;k++){
			printf("%x ",(unsigned)datos[k]);
		}
		printf("<CRC> %u </CRC> </inicio>\n",(unsigned)get_crc());
	}

	if (nuevo_actual==3){ /* Corto, en decimal. */
		printf("<i n=1 m=%d p=%lu> ",modo_actual,periodous_actual);
		for(k=0;k<QMUESTRAS;k++){
			//printf("%x ",datos[k]);
			printf("%u ",(unsigned)datos[k]);
		}
		printf("<v> %u </v> </i>\n",(unsigned)get_crc());
	}

	if (nuevo_actual==4){ /* Corto, en hexa. */
		printf("<i n=1 m=%d p=%lu> ",modo_actual,periodous_actual);
		for(k=0;k<QMUESTRAS;k++){
			printf("%x ",(unsigned)datos[k]);
		}
		printf("<v> %u </v> </i>\n",(unsigned)get_crc());
	}

}


void rutina_ya_conectado(){
	char caracter;
	char orden[100];
	unsigned int p_orden=0;
	do{
		// Reseteo de errores (principalmente OERR) del receptor USART.
		#asm
			bcf RCSTA, CREN
			bsf RCSTA, CREN
		#endasm		

		caracter = getc(); // Obtención del caracter presionado.
		if (caracter==CARACTER_FLUSH){
			p_orden=0;
		}else{
			// Caracter de control. Inicio del parseo.
			if (caracter == CARACTER_FIN_TRAMA){ 
				p_orden=0;
				if (parseo(orden)){
					borrar_buffer_muestras();
					lcd_muestreando();
					switch(modo_actual){
						case 0: iniciar_muestreo_sincrono(); 
								break;
						case 1: iniciar_muestreo_asincrono(); 
								break;
					}
					lcd_listo();
				}
			}else{
				orden[p_orden++]=caracter;
				orden[p_orden]=0;
				//printf("Orden: '%s' (%d).\n\r",orden,(int)caracter);
			}
		}
	}while(1);
}
