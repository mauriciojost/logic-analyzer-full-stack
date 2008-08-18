
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
        this.setBackground(Color.white);
        //this.setSize(100,100);
        this.canal = canal;
        this.dibujo = dibujo;        
    }
    
    public void cambiarRango(int[] muestras){

    }

    public void cambiarColor(Color color){

    }
    
    public void cambiarNombre(String nombre){

    }
    
    private void actualizarDibujo(){

    }

    public void update(Observable o, Object arg) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
