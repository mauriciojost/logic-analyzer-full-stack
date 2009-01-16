void inicializacion(){
	//lcd_init(); // Inicialización del módulo de LCD.

	//set_tris_b(0xFF); // original
	set_tris_d(0xFF);  /// OBOOOBOORRARRAARRRR SE USA PARA EL LCD!!!

	disable_interrupts(INT_EXT); // Configuración de las interrupciones.
	disable_interrupts(INT_RB); // Configuración de las interrupciones.
	enable_interrupts(GLOBAL);
}
