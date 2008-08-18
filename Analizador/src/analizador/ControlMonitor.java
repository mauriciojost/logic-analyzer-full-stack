package analizador;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Observable;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

public class ControlMonitor extends Observable{
    private int i=0; // Valores actuales de representación de muestras.
    private int f=1023;
    private Font fuenteEjes = new Font("Courier New", Font.BOLD,10);

    private static JPanel panel;
    
    public ControlMonitor(){
        panel = new JPanel(){
            @Override
            public void paintComponent(Graphics g){
                int cant=f-i+1;
                float espaciamiento = (float)panel.getWidth()/cant;
                g.setColor(Color.black);
                g.fillRect(0, 0, panel.getWidth(), panel.getHeight());
                g.setFont(fuenteEjes);
                g.setColor(Color.red);
                int intervalo =(cant/10)>0?(cant/10):1;
                for(int k=0; k<cant;k=k+intervalo){
                    g.drawLine((int)(espaciamiento*k+1), 0, (int)(espaciamiento*k+1), 5);
                    g.drawString(""+(k+i), (int)(espaciamiento*((0.1)+k)), 15);
                }
            }
        };
        
        panel.setSize(200,200);
        MouseAdapter myListener = new MouseAdapter(){
            private boolean iniciadoArrastre=false;
            private int x1;
            private int boton;
            public void mousePressed(MouseEvent e) {x1 = e.getX();boton=e.getButton();}
            public void mouseDragged(MouseEvent e) {iniciadoArrastre=true;}
            public void mouseReleased(MouseEvent e) {
                if (iniciadoArrastre){
                    if ((boton==MouseEvent.BUTTON1) && (e.getButton()==MouseEvent.BUTTON1)){
                        arrastre(x1,e.getX());
                    }
                }else{
                        clic(x1,e.getButton());
                }
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
    
        };   
        panel.addMouseListener(myListener);
        panel.addMouseMotionListener(myListener);
        panel.setBorder(new LineBorder(Color.white, 2, false));
        panel.setBackground(Color.white);
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

    int mapear(int pixelX){ // Mapea una posición de pixel en una posición de muestra de la señal (por ejemplo: pixel 33 -> muestra 1023).
        int qpixeles = panel.getWidth();
        int qmuestras = f-i+1;
        return (i+(int)((float)pixelX/qpixeles*qmuestras));   
    }
}
