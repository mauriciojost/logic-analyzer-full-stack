
package analizador;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JColorChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

public class ControlCanal extends JPanel implements Observer, MouseListener{

    private String nombre;
    private Canal canal;
    private Dibujo dibujo;
    private Color colorFondo = new Color(90,90,90);
    private Color colorNombre = new Color(204,204,204);
    private Font fuente = new Font("Arial", 1 , 10);
     
    public ControlCanal(Canal canal,Dibujo dibujo){
        this.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        this.setToolTipText("Haga clic con el botón DERECHO para cambiar de nombre del canal, y con el botón IZQUIERDO para cambiar el color del canal.");
        this.setBackground(colorFondo);
        this.canal = canal;
        this.dibujo = dibujo;
        this.nombre = "Canal "+ canal.obtenerId();
        this.addMouseListener(this);
    }
    
    public void cambiarRango(int i, int f){
        try{
            this.dibujo.cambiarRango(canal.obtenerRango(i, f));
        }catch(NullPointerException e){
            System.out.println("Canal no inicializado aún... ("+canal.obtenerId()+")");
        }
    }

    public void cambiarColor(Color color){
        dibujo.cambiarColor(color);
    }
    
    /*public void cambiarNombre(String nombre){
        dibujo.cambiarNombre(nombre);
    }*/
    
    private void actualizarDibujo(){
        dibujo.actualizarDibujo();
    }

    public void update(Observable o, Object arg) {
        int[] rango = (int[])arg;
        cambiarRango(rango[0],rango[1]);
    }

    public void mouseClicked(MouseEvent e){
        if (e.getButton()==e.BUTTON1){
            String str = JOptionPane.showInputDialog(null, "Nombre de Canal", nombre);
            if (str != null) {
                this.nombre = str;
                this.repaint();
            }            
        }
        else if (e.getButton()==e.BUTTON3){
            Color newColor = JColorChooser.showDialog(
                null,"Elija un color",
                this.getBackground());
            if (newColor != null) {
                this.setBackground(newColor);
                this.cambiarColor(newColor);
            }
        }
    }

    public void mousePressed(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}

    public void paintComponent(Graphics g){
        try{
            super.paintComponent (g);
            Graphics2D g2 = (Graphics2D)g;
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

            g2.setColor(colorNombre);
            g2.setFont(fuente);
            g2.drawString(nombre,32,23);

        }catch(NullPointerException e){
            System.out.println("Error - Parámetros no Inicializados");
        }
    }
}
