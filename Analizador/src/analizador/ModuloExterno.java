
package analizador;

import java.util.Observable;
import java.util.Random;

public class ModuloExterno extends Observable{
    private static ModuloExterno moduloExterno;
    private int estado;
    private boolean modo;
    private int veloc;
    private char[] señal;
    private char[] muestras;
    private Comunicador comunicador;
    
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
    }

    public void prueba(){
        señal = generarSeñalAleatoria();
       
        this.setChanged();
        this.notifyObservers(señal);
    }

    private char[] generarSeñalAleatoria(){ // Sólo a los efectos de la depuración.
        char[] rango = new char[1024];
        Random a = new Random();
        for (int k=0;k<1024;k++){
            rango[k] = (char)(a.nextInt());
        }
        return rango;
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

    public static ModuloExterno getModuloExterno(){
        return moduloExterno;
    }
}
