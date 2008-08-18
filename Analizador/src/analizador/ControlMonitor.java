
package analizador;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Observable;
import javax.swing.JPanel;

public class ControlMonitor extends Observable{
    
    private int i;
    private int f;
    private int zoom;
    private JPanel panel;
    
    public ControlMonitor(){
        panel = new JPanel(){
            @Override
            public void paintComponent(Graphics g){
                               
            }
            
        };
        panel.setBackground(Color.white);
        panel.setSize(200,200);
        MouseAdapter myListener = new MouseAdapter(){
            @Override
            public void mousePressed(MouseEvent e) {
                int x = e.getX();
                int y = e.getY();
            }

            @Override
            public void mouseDragged(MouseEvent e) {
                updateSize(e);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                updateSize(e);
            }

            void updateSize(MouseEvent e) {
                int x = e.getX();
                int y = e.getY(); 
            }
        };
        
        panel.addMouseListener(myListener);
        panel.addMouseMotionListener(myListener);
        
    }

    public JPanel getPanel(){
        return panel;
    }
    
    public void desplazarTodo(int desp){
        
    }
    public void zoomTodo(int zoom){
        
    }
    
    public void dibujarEscala(){
        
    }
}
