/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package analizador;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JPanel;

/**
 *
 * @author Administrador
 */
public class Dibujo extends JPanel{
    
    int[] muestras;
    Color color;
    String nombre;
    
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
        int width_bit = 20; // this.getWidth() / muestras.length;
        int heigth_bit = 20; 
        g2.drawString(nombre,20,this.getHeight()/2);
        for (int i = 0; i<muestras.length;i++){
            if (i>0 && muestras[i]!=muestras[i-1]) /* Si hay un cambio de signo.. */  
                    g2.drawLine(width_bit*i, heigth_bit ,width_bit*(i+1),0); /* dibujo una linea vertical antes de.. */
            g2.drawLine(width_bit*i,muestras[i]*heigth_bit ,width_bit*(i+1),muestras[i]*heigth_bit); /* dibujar la proxima linea horizontal*/
        }
    }

}
