/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package analizador;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Line2D;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

/**
 *
 * @author Administrador
 */
public class Dibujo extends JPanel{
    
    private int[] muestras = {1,0,1,0};
    private Color color = Color.RED;
    private String nombre = "Canal1";
    private int ancho_bit = 25; // this.getWidth() / muestras.length;
    private int altura_bit = 50;
    private int altura_eje = 60;
    private int ancho_eje = 60;
    
    public Dibujo(){
        super();
        this.setBackground(Color.BLACK);
    }
    
    public void cambiarRango(int[] muestras){
        this.muestras = muestras;
        repaint();
    }

    public void cambiarColor(Color color){
        this.color = color;
        repaint();
    }
    
    public void cambiarNombre(String nombre){
        this.nombre = nombre;
        repaint();
    }

    private void actualizarDibujo(){
        repaint();
    }
    
    @Override
    public void paintComponent(Graphics g){
        super.paintComponent (g);
        Graphics2D g2 = (Graphics2D)g;
        
        g2.setColor(color);
        g2.drawString(nombre,5,altura_eje-(altura_bit/2));
        for (int i = 0; i<muestras.length;i++){
            System.out.println(muestras[i]);
            if (i>0 && muestras[i]!=muestras[i-1]){ // Si hay un cambio de signo..   
                g2.drawLine(ancho_eje + ancho_bit*i, altura_eje - altura_bit ,ancho_eje+ancho_bit*i, altura_eje); // dibujo una linea vertical antes de..
            }
            g2.drawLine(ancho_eje+ancho_bit*i,altura_eje - muestras[i]*altura_bit ,ancho_eje+ancho_bit*(i+1), altura_eje - muestras[i]*altura_bit); // dibujar la proxima linea horizontal
        }
        /*Graphics2D g2 = (Graphics2D) g;
    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

    g2.setPaint(Color.gray);
    int x = 5;
    int y = 7;

    g2.draw(new Line2D.Double(x, y, 200, 200));
    g2.drawString("Line2D", x, 250);*/
    }
    
}
