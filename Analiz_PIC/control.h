void inicializacion(){
	lcd_iniciar();
	set_tris_b(0xFF); // original
	lcd_presentacion();
	disable_interrupts(INT_EXT); // Configuraci�n de las interrupciones.
	disable_interrupts(INT_RB); // Configuraci�n de las interrupciones.
	enable_interrupts(GLOBAL);
}
