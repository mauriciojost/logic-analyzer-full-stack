void consola_iniciar_muestreo(){
	iniciar_muestreo();
	lcd_listo();
}

void consola_ver_muestras(){
	responder_trama();
}

void consola_establecer_configuracion(){
	printf("\n\rEstableciendo configuracion...");
	printf("\n\r        Modo (0 sincrono-periodico, 1 asincrono)? %u-> ",(unsigned int)modo_actual); // Mensaje en pantalla de PC.
	modo_actual = get_int(); // Obtención del valor ingresado.
	if (modo_actual==0){
		printf("\n\r        Periodo [uS]? %luuS-> ",periodous_actual); // Mensaje en pantalla de PC.
		periodous_actual = get_long();
		printf("\n\r        Hecho! (modo periodico, periodo %luuS).", periodous_actual); // Mensaje en pantalla de PC.
	}else{
		modo_actual=1;
		printf("\n\r        Hecho! (modo asincronico)."); // Mensaje en pantalla de PC.
	}
	nuevo_actual=0;
}


void modo_consola(){
	char solicitud=2;
	while (solicitud!=0){
		printf("\n\r\n\r");
		printf("************************************\n\r");
		printf("             MODO CONSOLA\n\r");
		printf("************************************\n\r\n\r");
		printf("Opciones:\n\r");
		printf("  . (S)alir del modo consola.\n\r");
		printf("  . (C)onfigurar muestreo.\n\r");
		printf("  . (I)niciar muestreo.\n\r");
		printf("  . (V)isualizar muestras.\n\r");
		printf("  Seleccion: ");
		solicitud = getc();
		printf("%c\n\r",solicitud);
		switch(solicitud){
			case 'S': case 's': printf("\n\rSalida del modo consola.\n\r"); return;
			case 'C': case 'c': consola_establecer_configuracion(); break;
			case 'I': case 'i': printf("\n\rMuestreando...\n\r");lcd_muestreando(); consola_iniciar_muestreo(); lcd_listo(); break;
			case 'V': case 'v': consola_ver_muestras(); lcd_listo(); break;
			default: printf("Opcion no valida (%x).\n\r",solicitud);
		}
		
	}
}

