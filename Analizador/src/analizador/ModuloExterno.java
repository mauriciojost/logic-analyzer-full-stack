
package analizador;

import java.util.Observable;

public class ModuloExterno extends Observable{
    
    private int estado;
    private boolean modo;
    private int veloc;
    private char[] muestras;
    private Comunicador comunicador;
    
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
    }
    public int obtenerEstado(){
        return 0;
    }
    
    public void iniciarMuestreo(){
        
    }
    
    public void cambiarModo(boolean modo){
        
    }
    
    public void cambiarVelocidad(int veloc){
        
    }
    private void notificarMuestras(char[] muestras){
        
    }

}
