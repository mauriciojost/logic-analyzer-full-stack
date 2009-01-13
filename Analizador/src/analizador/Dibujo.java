package analizador;

import java.awt.*;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.LineBorder;

public class Dibujo extends JPanel{
    
    private int canalID;
    private int[] señal;// = {1,0,1};
    private Color colorSeñal = new Color(255,120,120);
    private Color colorFondo = Color.black;
    private Color colorBorde = new Color(100,100,100);
    private Color colorNombre = new Color(204,204,204);
    private Color colorEjesRef = new Color(70,70,70);
    private Font fuente = new Font("Arial", 0, 12);
    private String nombre;
    private float ancho_bit; 
    private int altura_bit;
    private int referenciaY;
    private int desplazamientoHorizontal;
    
    public Dibujo(int canal){
        super();
        Border a = new LineBorder(colorBorde,1,true);
        this.setBorder(a); // Mauri: puesto para poder cambiarlo en un futuro (no tengo tan mal gusto).
        this.canalID=canal;
        this.setBackground(colorFondo);
        this.cambiarRango(señal);
        this.nombre = "Canal "+ canal;
    }
        
    public void cambiarRango(int[] señal){
        this.señal = señal;
        actualizarDibujo();
    }

    public void cambiarColor(Color color){
        this.colorSeñal = color;
        actualizarDibujo();
    }
    
    public void cambiarNombre(String nombre){
        this.nombre = nombre;
        actualizarDibujo();
    }

    public void actualizarDibujo(){
        repaint();
    }
    
    /* Estos metodo deben ejecutarse despues 
     de que se inicialice la altura del Jpanel*/
    private void configurarReferencias(){
        try{
            referenciaY = this.getHeight()-5;
            desplazamientoHorizontal = 0;
            altura_bit = this.getHeight()-10;
            ancho_bit = (float)(this.getWidth()-desplazamientoHorizontal)/señal.length; 
        }catch(NullPointerException e){
            System.out.println("Este objeto Dibujo ("+this.canalID+") se encuentra vacío.");
        }
    }
    
    @Override
    public void paintComponent(Graphics g){
        try{
            super.paintComponent (g);
            Graphics2D g2 = (Graphics2D)g;
            //g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
            configurarReferencias();

            g2.setColor(colorNombre);
            g2.setFont(fuente);
            //g2.drawString(nombre,3,referenciaY-15);

            g2.setColor(colorSeñal);
            for (int i = 0; i<señal.length;i++){
                g2.setColor(colorEjesRef);
                g2.drawLine(desplazamientoHorizontal + (int)(ancho_bit*i), referenciaY - altura_bit ,desplazamientoHorizontal+(int)(ancho_bit*i), referenciaY); // dibujo una linea vertical antes de..
                g2.setColor(colorSeñal);
                if (i>0 && señal[i]!=señal[i-1]){ // Si hay un cambio de valor..   
                    g2.drawLine(desplazamientoHorizontal + (int)(ancho_bit*i), referenciaY - altura_bit ,desplazamientoHorizontal+(int)(ancho_bit*i), referenciaY); // dibujo una linea vertical antes de..
                }
                g2.drawLine(desplazamientoHorizontal+(int)(ancho_bit*i),referenciaY - señal[i]*altura_bit ,desplazamientoHorizontal+(int)(ancho_bit*(i+1)), referenciaY - señal[i]*altura_bit); // dibujar la proxima linea horizontal
            }
        }catch(NullPointerException e){
            System.out.println("Este objeto Dibujo ("+this.canalID+") se encuentra vacío.");
        }
    }
    
}
