package analizador;

import java.util.Observable;
import java.util.Observer;
import java.util.Random;

public class Canal implements Observer{
       
    private char[] señal;
    private int estado;
    
    public Canal(){
        señal = generarSeñalAleatoria();
    }
    
    private char[] generarSeñalAleatoria(){ // Sólo a los efectos de la depuración.
        char[] rango = new char[1024];
        Random a = new Random();
        for (int k=0;k<1024;k++){
            rango[k] = (char)(a.nextBoolean()?1:0);
        }
        return rango;
    }
    
    public void notify(char[] muestras){
        
    }
    
    public int[] obtenerRango(int i, int f){
        i = ((i<0)?0:i);
        f = ((f>1023)?1023:f);
        int cant = f-i+1;
        int bits[] = new int[cant];
        for (int k=0;k<cant;k++){
            bits[k]=señal[k+i];
        }
        return bits;
    }
    
    public int obtenerEstado(){
        return 0; // Por ahora consideramos que el estado 0 es el de inicialización correcta y muestreo hecho.
    }

    public void update(Observable o, Object arg) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
