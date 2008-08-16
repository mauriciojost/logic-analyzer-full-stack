
package analizador;

import java.util.Observable;
import java.util.Observer;

public class Canal implements Observer{
       
    private char[] señal;
    private int estado;
    

    public void notify(char[] muestras){
        
    }
    
    public int[] obtenerRango(int i, int f){
        return null;
    }
    
    public int obtenerEstado(){
        return 0;
    }

    public void update(Observable o, Object arg) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

}
