
void modo_consola(){
	printf("\n\r        Modo (0 periodico, 1 asincrono)? %u-> ",(unsigned int)modo_actual); // Mensaje en pantalla de PC.
	modo_actual = get_int(); // Obtención del valor ingresado.
	if (modo_actual==0){
		printf("\n\r        Periodo? %luuS-> ",periodous_actual); // Mensaje en pantalla de PC.
		periodous_actual = get_long();
		printf("\n\r        Hecho! (modo periódico, periodo %luuS).", periodous_actual); // Mensaje en pantalla de PC.
	}else{
		printf("\n\r        Hecho! (modo asincronico)."); // Mensaje en pantalla de PC.
	}
	
}
