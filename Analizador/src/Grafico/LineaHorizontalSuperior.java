package Grafico;

import java.awt.Color;
import java.awt.Graphics2D;
import javax.swing.JPanel;

public class LineaHorizontalSuperior extends Grafico{
    
     public LineaHorizontalSuperior (Integer x0, Integer y0, Integer w0, Integer h0, Color c, JPanel p) {
        super(x0, y0,w0,h0,c,p);
     }
        
    @Override
    public void dibujate(Graphics2D g2) {
        g2.setColor(color);
        g2.fillRect(x, y, w, h);
    }
}