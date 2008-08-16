
package analizador;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JPanel;

public class Dibujo extends JPanel{
    
    private int[] muestras = {1,0,1,0};
    private Color color = Color.RED;
    private String nombre = "Canal1";
    private int ancho_bit; 
    private int altura_bit;
    private int altura_eje;
    private int ancho_eje;
    
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
    
    private void setLayout1(){
        ancho_bit = 25; 
        altura_bit = 50;
        altura_eje = 60;
        ancho_eje = 60;
    }
    /* Estos metodo deben ejecutarse despues 
     de que se inicialice la altura del Jpanel*/
    private void setLayout2(){
        ancho_bit = 25; 
        altura_bit = this.getHeight()-10;
        altura_eje = this.getHeight()-5;
        ancho_eje = 60;
    }
    
    @Override
    public void paintComponent(Graphics g){
        super.paintComponent (g);
        Graphics2D g2 = (Graphics2D)g;
        //setLayout1();
        setLayout2();
        g2.setColor(color);
        g2.drawString(nombre,5,altura_eje-(altura_bit/2));
        for (int i = 0; i<muestras.length;i++){
            System.out.println(muestras[i]);
            if (i>0 && muestras[i]!=muestras[i-1]){ // Si hay un cambio de signo..   
                g2.drawLine(ancho_eje + ancho_bit*i, altura_eje - altura_bit ,ancho_eje+ancho_bit*i, altura_eje); // dibujo una linea vertical antes de..
            }
            g2.drawLine(ancho_eje+ancho_bit*i,altura_eje - muestras[i]*altura_bit ,ancho_eje+ancho_bit*(i+1), altura_eje - muestras[i]*altura_bit); // dibujar la proxima linea horizontal
        }
    }
    
}
