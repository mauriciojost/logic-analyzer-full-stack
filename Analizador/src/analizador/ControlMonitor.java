package analizador;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.Observable;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

public class ControlMonitor extends Observable{
    private int i=0; // Valores actuales de representación de muestras.
    private int f=1023;
    private Font fuenteEjes = new Font("Arial", Font.BOLD,9);
    private Color colorEjes = new Color(110,110,110);
    private Color colorFlechas = new Color(150,255,150);
    private Color colorFondo = new Color(90,90,90);
    private Color colorBorde = new Color(110,110,110);
    private Color colorLeyenda = Color.yellow;
    private static JPanel panel;
    private static ControlMonitor controlMonitor;
    private boolean shift_presionado = false;
    
    public ControlMonitor(){
        ControlMonitor.controlMonitor = this;
        panel = new JPanel(){
            @Override
            public void paintComponent(Graphics g1){
                int cant=Math.abs(f-i+1);
                Graphics2D g = (Graphics2D) g1;
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
                float espaciamiento = (float)panel.getWidth()/cant;
                g.setColor(colorFondo);
                g.fillRect(0, 0, panel.getWidth(), panel.getHeight());
                g.setFont(fuenteEjes);
                g.setColor(colorEjes);
                for(int k=0; k<cant;k=k+1){
                    g.drawLine((int)(espaciamiento*k+1), 0, (int)(espaciamiento*k+1), 5);
                }
                g.setColor(colorLeyenda);
                int intervalo =(cant/10)>0?(cant/10):1;
                for(int k=0; k<cant;k=k+intervalo){
                    g.drawString(""+(k+i), (int)(espaciamiento*k), 15);
                }
            }
        };
        
        KeyListener myListener_teclado = new KeyListener(){

            public void keyTyped(KeyEvent e) {}

            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode()==e.VK_SHIFT) {
                    shift_presionado = true;
                    System.out.println("Shift presionado...");
                }
            }

            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode()==e.VK_SHIFT) {
                    shift_presionado = false;
                    System.out.println("Chau shift...");
                }
            }
        
        };
        
        /* GUILLE
         * Ahora tenés visibilidad a una variable shift_presionado. 
         * Es booleana. True se está shift presionado.
         * Cuando se mueva la ruedita, preguntás si está o no el shift.
         * Si está hacés desplazamiento.
         * Si no está hacés zoom.
         */
        
        
        panel.addKeyListener(myListener_teclado);
        panel.setFocusable(true);
        
        panel.setBorder(new LineBorder(colorBorde,1));
        
        MouseAdapter myListener = new MouseAdapter(){
            private boolean iniciadoArrastre=false;
            private int x1, boton; // Posición X y botón de inicio del clic.
            public void mousePressed(MouseEvent e) {x1 = e.getX();boton=e.getButton();}
            public void mouseDragged(MouseEvent e) {
                iniciadoArrastre=true;
                Graphics g = panel.getGraphics();
                g.setColor(colorFondo);
                g.fillRect(1, 15, panel.getWidth()-2, 10); // Borrado.
                g.setColor(colorFlechas);
                g.drawLine(x1, 20, e.getX(), 20);g.drawLine(x1, 18, x1, 22);g.drawLine(e.getX(), 18, e.getX(), 22); // Pintado de las flechas indicadoras de zoom.
                
            }
            public void mouseReleased(MouseEvent e) {
                if (iniciadoArrastre){
                    if ((boton==MouseEvent.BUTTON1) && (e.getButton()==MouseEvent.BUTTON1)){arrastre(x1,e.getX());}
                }else{clic(x1,e.getButton());}
                iniciadoArrastre=false;
            }
            private void clic(int pixelX, int boton){
                int qmuestras=f-i+1, muestra=mapear(pixelX), largo=(int)(qmuestras/4);

                if (boton==MouseEvent.BUTTON1){ // Zoom In
                    i=muestra-largo;f=muestra+largo;nuevoRango(i,f);
                }else{ // Zoom Out           
                    i-=(largo+1);f+=(largo+1);nuevoRango(i,f);
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

        panel.addMouseWheelListener(
            new MouseWheelListener() {
                public void mouseWheelMoved(MouseWheelEvent mwe) {
                    if (shift_presionado) {
                        //zoom...
                    } else {
                    desplazarTodo(mwe.getWheelRotation());
                    }
		}
            }
	);

    }
    public JPanel getPanel(){
        return panel;
    }
    
    public static ControlMonitor getControlMonitor(){
        return controlMonitor;
    }
    
    public void nuevoRango(int i, int f){
        //if (Math.abs(i-f) > 10){         // ESTOY TOCANDO YO! :D
            this.i=(i<0)?0:i;
            this.f=(f>1023)?1023:f;
            panel.repaint();
            this.setChanged();
            int datos[] = {i,f};
            this.notifyObservers(datos);
        //}
    }
    
    public void inicializar(){
        nuevoRango(0,1023);
    }
    
    public void desplazarTodo(int desp){
        nuevoRango(i+desp,f+desp);
    }
    public void zoomTodo(int zoom){
        int qmuestras=f-i+1, muestra=i+(f-i)/2, largo=(int)(qmuestras/4);
        if (zoom>0){
            i=muestra-largo;f=muestra+largo;nuevoRango(i,f);
        }else{
            i-=(largo+1);f+=(largo+1);nuevoRango(i,f);
        }
        nuevoRango(i, f);
    }
    public void dibujarEscala(){
        panel.repaint();
    }
    
}
