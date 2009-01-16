void inicializacion(){
	lcd_init(); // Inicializaci�n del m�dulo de LCD.

	set_tris_b(0xFF); // original

	printf(lcd_putc,"\fInicio OK.");
	disable_interrupts(INT_EXT); // Configuraci�n de las interrupciones.
	disable_interrupts(INT_RB); // Configuraci�n de las interrupciones.
	enable_interrupts(GLOBAL);
}
