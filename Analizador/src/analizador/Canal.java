
package analizador;

import java.util.Observable;
import java.util.Observer;
import java.util.Random;

public class Canal implements Observer{
       
    private char[] señal;
    private int estado;
    
    private int[] generarRangoAleatorio(int i, int f){ // Sólo a los efectos de la depuración.
        int cant = f-i+1;
        int[] rango = new int[cant];
        Random a = new Random();
        for (int k=0;k<cant;k++){
            rango[i] = (a.nextBoolean()?1:0);
        }
        return rango;
    }
    
    public void notify(char[] muestras){
        
    }
    
    public int[] obtenerRango(int i, int f){
        return generarRangoAleatorio(i,f);
    }
    
    public int obtenerEstado(){
        return 0;
    }

    public void update(Observable o, Object arg) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
