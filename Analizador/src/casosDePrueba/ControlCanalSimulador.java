
package casosDePrueba;

import analizador.*;

import java.awt.Color;
import java.util.Random;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class ControlCanalSimulador extends JPanel{
    private Dibujo dibujo;
    private char[] señal;
    
    public ControlCanalSimulador(Dibujo dibujo){
        this.dibujo = dibujo;
        this.add(dibujo);
        this.setLayout(null);
    }
    
    public char[] obtenerRango(int i, int f){
        int cant = f-i+1;
        char bits[] = new char[cant];
        for (int k=0;k<cant;k++){
            bits[k]=señal[k+i];
        }
        return bits;
    }
    
    public void inicializar_señal(int n){
        señal = new char[n];
        for (int i=0; i<n; i++){
            señal[i] = ((new Random()).nextBoolean())?(char)1:(char)0;
        }
    }
    
    public static void main(String args[]){
        
        Random a = new Random();
        
        /* Creacion de una ventana. */
        JFrame ventana = new JFrame("ControlCanalSimulador");
        ventana.setBounds(0, 0, 900, 200);
        ventana.setVisible(true);
        ventana.setLayout(null);
        
        /* Creacion de un Dibujo y su Caso de Prueba. */
        Dibujo dibujo = new Dibujo(1);
        ControlCanalSimulador simulador = new ControlCanalSimulador(dibujo);
        simulador.setBounds(50,0,800,200);
        simulador.setBackground(simulador.getBackground().darker());
        
        
        simulador.inicializar_señal(1024);
        ventana.add(simulador);
        dibujo.setBounds(50,50,700,100);
        for(int j=1023;j>100;j=j-1){
            dibujo.cambiarRango(simulador.obtenerRango(0, j));
            try{Thread.sleep(20);}catch(Exception e){e.printStackTrace();}
            dibujo.actualizarDibujo();
        }
        try{Thread.sleep(1000);}catch(Exception e){e.printStackTrace();}
        for(int j=0;j<800;j=j+1){
            dibujo.cambiarRango(simulador.obtenerRango(j, 100+j));
            try{Thread.sleep(20);}catch(Exception e){e.printStackTrace();}
            dibujo.actualizarDibujo();
            
            if (j%100==0){
                dibujo.cambiarColor(Color.getHSBColor(a.nextFloat(), a.nextFloat(), a.nextFloat()).brighter().brighter());
            }
        }
        try{Thread.sleep(1000);}catch(Exception e){e.printStackTrace();}
        
        
        System.exit(0);
        
        

    }
}
