package analizador;

import java.util.Observable;
import java.util.Observer;

public class Canal implements Observer{
       
    private char[] señal;
    private int estado;
    
    public Canal(){
        
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
        return this.estado; // Por ahora consideramos que el estado 0 es el de inicialización correcta y muestreo hecho.
    }

    public void update(Observable o, Object arg) {
        this.señal = (char[])arg;
    }

}
