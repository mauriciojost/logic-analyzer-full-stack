package analizador;

import java.awt.*;
import java.awt.event.*;
import java.util.Observable;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

public class ControlMonitor extends Observable{
    private int i=0; // Valores actuales de representación de muestras.
    private int f=1023;
    private Font fuenteEjes = new Font("Arial", Font.BOLD,8);
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
                //g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
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
                switch (e.getKeyCode()){
                    case (KeyEvent.VK_SHIFT):
                        shift_presionado = true; break;
                    case (KeyEvent.VK_UP):
                        zoomTodo(1); break;
                    case (KeyEvent.VK_DOWN):
                        zoomTodo(-1); break;
                    case (KeyEvent.VK_LEFT):
                        desplazarTodo(-1); break;
                    case (KeyEvent.VK_RIGHT):
                        desplazarTodo(1); break;
                    case (KeyEvent.VK_PAGE_DOWN):
                        desplazarTodo(10); break;
                    case (KeyEvent.VK_PAGE_UP):
                        desplazarTodo(-10); break;
                }
                
            }

            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode()==e.VK_SHIFT) {
                    shift_presionado = false;
                }
            }
        
        };
        
        panel.addKeyListener(myListener_teclado);
        panel.setFocusable(true);
        panel.setCursor(Cursor.getPredefinedCursor(Cursor.W_RESIZE_CURSOR));
        panel.setToolTipText("Seleccione la zona deseada para hacer ZOOM.");
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
                        zoomTodo(mwe.getWheelRotation());
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
        
        if (i>f){int aux; aux=f;f=i;i=aux;}
        
        i=(i<0)?0:((i>1023)?1023:i);
        f=(f<0)?0:((f>1023)?1023:f);
        
        if (Math.abs(i-f) < 3){    
            int valor = 3-Math.abs(i-f);
            if (i!=0) i-=valor;
            if (f!=1023) f+=valor;
        }
        this.i = i;
        this.f = f;
        panel.repaint();
        this.setChanged();
        int datos[] = {i,f};
        this.notifyObservers(datos);
    }
    
    public void inicializar(){
        nuevoRango(0,1023);
    }
    
    public void desplazarTodo(int desp){
        if ((!((f+desp)>1023)) && (!(i+desp<0))){
            {nuevoRango(i+desp,f+desp);}
        }
    }
    public void zoomTodo(int zoom){
        int qmuestras=f-i+1, muestra=i+(f-i)/2, largo=(int)(qmuestras/4);
        if (zoom>0){
            i=muestra-largo;f=muestra+largo;
        }else{
            i-=(largo+1);f+=(largo+1);
        }
        nuevoRango(i,f);
    }
    public void dibujarEscala(){
        panel.repaint();
    }
    
}
