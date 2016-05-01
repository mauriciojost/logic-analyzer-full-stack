package casosDePrueba;

import java.util.Observable;
import java.util.Observer;

class CanalSimulador implements Observer{
       
    private char[] señal;
    private int estado = 0;
    private int id;
    
    public CanalSimulador(int i){
        this.id = i;
    }

    public char[] obtenerRango(int i, int f){
        i = ((i<0)?0:i);
        f = ((f>señal.length-1)?señal.length-1:f);
        int cant = f-i+1;
        char bits[] = new char[Math.abs(cant)];
        for (int k=0;k<cant;k++){
            bits[k]=señal[k+i];
        }
        return bits;
    }
    
    public int obtenerEstado(){
        return this.estado; // Por ahora consideramos que el estado 1 es el muestreo hecho.
    }

    public int obtenerId(){
        return this.id; // Por ahora consideramos que el estado 0 es el de inicialización correcta y muestreo hecho.
    }
    
    public void update(Observable o, Object arg) {
        int i;
        char[] argumento = (char[])arg;
        this.señal = new char[argumento.length];
        
        System.out.println("Señal Actualiza en el Canal");
        
        for (i=0;i<argumento.length;i++){
            this.señal[i] = (char)((argumento[i]>>this.id)&1);
            //System.out.print(señal[i]);
        }
        //System.out.print("\n");
        estado = 1;
    }

}