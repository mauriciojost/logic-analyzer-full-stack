
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
    
    public void inicializar_señal(){
        señal = new char[1024];
        for (int i=0; i<1024; i++){
            señal[i] = ((new Random()).nextBoolean())?(char)1:(char)0;
        }
    }
    
    
    public void inicializar_señal_tipo(){
        señal = new char[1024];
        
        señal[0] = 1;
        señal[1] = 1;
        señal[2] = 1;
        señal[3] = 0;
        señal[4] = 0;
        señal[5] = 1;
        señal[6] = 0;
        señal[7] = 1;
        señal[8] = 1;
        señal[9] = 1;
        
        for (int i=10; i<1024; i++){
            señal[i] = 0;
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
        
        
        simulador.inicializar_señal();
        ventana.add(simulador);
        dibujo.setBounds(50,50,700,100);
        for(int j=1023;j>100;j=j-1){
            dibujo.cambiarRango(simulador.obtenerRango(0, j));
            try{Thread.sleep(10);}catch(Exception e){e.printStackTrace();}
            dibujo.actualizarDibujo();
        }
        try{Thread.sleep(1000);}catch(Exception e){e.printStackTrace();}
        for(int j=0;j<500;j=j+1){
            dibujo.cambiarRango(simulador.obtenerRango(j, 100+j));
            try{Thread.sleep(10);}catch(Exception e){e.printStackTrace();}
            dibujo.actualizarDibujo();
            
            if (j%100==0){
                dibujo.cambiarColor(Color.getHSBColor(a.nextFloat(), a.nextFloat(), a.nextFloat()).brighter().brighter());
            }
        }
        
        
        try{Thread.sleep(1000);}catch(Exception e){e.printStackTrace();}
        System.out.println("Test de documentacion.");
        simulador.inicializar_señal_tipo();
        try{Thread.sleep(2000);}catch(Exception e){e.printStackTrace();}
        System.out.println("Rango 0:1023. Color rosa.");
        dibujo.cambiarRango(simulador.obtenerRango(0, 1023));
        dibujo.cambiarColor(Color.pink);
        try{Thread.sleep(6000);}catch(Exception e){e.printStackTrace();}
        
        System.out.println("Rango 4:7. Color rosa.");
        dibujo.cambiarRango(simulador.obtenerRango(4, 7));
        try{Thread.sleep(6000);}catch(Exception e){e.printStackTrace();}
        
        System.out.println("Desplazamiento en -2.");
        dibujo.cambiarRango(simulador.obtenerRango(2, 5));
        dibujo.cambiarColor(Color.pink);
        try{Thread.sleep(6000);}catch(Exception e){e.printStackTrace();}
        
        System.out.println("Color azul.");
        dibujo.cambiarColor(Color.blue.brighter());
        try{Thread.sleep(6000);}catch(Exception e){e.printStackTrace();}
        
        System.exit(0);
        
        

    }
}
