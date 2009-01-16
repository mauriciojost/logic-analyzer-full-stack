
void lcd_muestreando(){
	switch (modo_actual){
		case 0: printf(lcd_putc,"\fM. Sinc. %luuS\nMuestreando...",(unsigned long)periodous_actual); break;
		case 1: printf(lcd_putc,"\fM. Asinc.\nMuestreando..."); break;
	}
}

void lcd_listo(){
	lcd_gotoxy(1,2);
	printf(lcd_putc,"Listo.          ");
}

void lcd_presentacion(){
	printf(lcd_putc,"\fSistema iniciado");
	delay_ms(2000);
	printf(lcd_putc,"\fFCEFyN - UNC");
	delay_ms(1000);
	lcd_gotoxy(1,2); printf(lcd_putc,"      Ing. Comp.");
}

void lcd_transmitiendo(){
	lcd_gotoxy(1,2);
	printf(lcd_putc,"Transmitiendo...");
}