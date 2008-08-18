
package analizador;

import java.awt.Color;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JPanel;

public class ControlCanal extends JPanel implements Observer{

    private String nombre;
    private Canal canal;
    private Dibujo dibujo;
     
    public ControlCanal(Canal canal,Dibujo dibujo){
        this.setBackground(Color.gray);
        this.canal = canal;
        this.dibujo = dibujo;        
    }
    
    public void cambiarRango(int i, int f){
        this.dibujo.cambiarRango(canal.obtenerRango(i, f));
    }

    public void cambiarColor(Color color){
        dibujo.cambiarColor(color);
    }
    
    public void cambiarNombre(String nombre){
        dibujo.cambiarNombre(nombre);
    }
    
    private void actualizarDibujo(){
        dibujo.actualizarDibujo();
    }

    public void update(Observable o, Object arg) {
        int[] rango = (int[])arg;
        cambiarRango(rango[0],rango[1]);
    }
}
