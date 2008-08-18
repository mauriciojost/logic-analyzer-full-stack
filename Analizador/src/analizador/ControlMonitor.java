package analizador;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Observable;
import javax.swing.JPanel;

public class ControlMonitor extends Observable{
    private int i=0; // Valores actuales de representación de muestras.
    private int f=1023;
    private Font fuenteEjes = new Font("Courier New", Font.BOLD,10);
    private Color colorEjes = Color.red;
    private Color colorFondo = Color.black;
    private static JPanel panel;
    
    public ControlMonitor(){
        panel = new JPanel(){
            @Override
            public void paintComponent(Graphics g){
                int cant=f-i+1;
                float espaciamiento = (float)panel.getWidth()/cant;
                g.setColor(colorFondo);
                g.fillRect(0, 0, panel.getWidth(), panel.getHeight());
                g.setFont(fuenteEjes);
                g.setColor(colorEjes);
                for(int k=0; k<cant;k=k+1){
                    g.drawLine((int)(espaciamiento*k+1), 0, (int)(espaciamiento*k+1), 5);
                }
                int intervalo =(cant/10)>0?(cant/10):1;
                for(int k=0; k<cant;k=k+intervalo){
                    g.drawString(""+(k+i), (int)(espaciamiento*k), 15);
                }
            }
        };
        
        panel.setSize(200,200);
        MouseAdapter myListener = new MouseAdapter(){
            private boolean iniciadoArrastre=false;
            private int x1, boton; // Posición X y botón de inicio del clic.
            public void mousePressed(MouseEvent e) {x1 = e.getX();boton=e.getButton();}
            public void mouseDragged(MouseEvent e) {
                iniciadoArrastre=true;
                Graphics g = panel.getGraphics();
                g.setColor(colorFondo);
                g.fillRect(0, 15, panel.getWidth(), 10); // Borrado
                g.setColor(colorEjes);
                g.drawLine(x1, 20, e.getX(), 20);
                g.drawLine(x1, 18, x1, 22);
                g.drawLine(e.getX(), 18, e.getX(), 22);
                
            }
            public void mouseReleased(MouseEvent e) {
                if (iniciadoArrastre){
                    if ((boton==MouseEvent.BUTTON1) && (e.getButton()==MouseEvent.BUTTON1)){arrastre(x1,e.getX());}
                }else{clic(x1,e.getButton());}
                iniciadoArrastre=false;
            }
            private void clic(int pixelX, int boton){
                int qmuestras = f-i+1;
                int muestra = mapear(pixelX);
                int largo =(int)(qmuestras/4);

                if (boton==MouseEvent.BUTTON1){ // Zoom In
                    i=muestra-largo;f=muestra+largo;
                    nuevoRango(i,f);
                }else{ // Zoom Out           
                    i-=(largo+1);f+=(largo+1);
                    nuevoRango(i,f);
                }
            }
            private void arrastre(int desdePixelX,int hastaPixelX){
                int pto1=mapear(desdePixelX), pto2=mapear(hastaPixelX);
                int desde = (pto1>pto2)?pto2:pto1; // Toma el menor.
                int hasta = pto1+pto2-desde; // Toma el otro.
                nuevoRango(desde,hasta);
            }
            int mapear(int pixelX){ // Mapea una posición de pixel en una posición de muestra de la señal (por ejemplo: pixel 33 -> muestra 1023).
                int qpixeles = panel.getWidth();
                int qmuestras = f-i+1;
                return (i+(int)((float)pixelX/qpixeles*qmuestras));   
            }
        };   
        panel.addMouseListener(myListener);
        panel.addMouseMotionListener(myListener);
    }
    public JPanel getPanel(){
        return panel;
    }
    
    public void nuevoRango(int i, int f){
        this.i=(i<0)?0:i;//System.out.println("nuevoRango...(Antes)("+this.i+","+this.f+")");
        this.f=(f>1023)?1023:f;
        panel.repaint();
        this.notifyObservers();
    }
    public void desplazarTodo(int desp){
        
    }
    public void zoomTodo(int zoom){
        
    }
    public void dibujarEscala(){
        panel.repaint();
    }
    
}
