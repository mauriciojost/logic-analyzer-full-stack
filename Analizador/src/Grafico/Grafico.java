package Grafico;

import java.awt.Color;
import java.awt.Graphics2D;
import javax.swing.JPanel;

public abstract class Grafico {
    protected Integer x, y, w, h;
    protected Integer xAnterior,yAnterior;
    protected final Integer xInicial,yInicial;
    protected Color color;
    protected JPanel panel;
        
    public Grafico(Integer x0, Integer y0, Integer w0, Integer h0, Color c, JPanel p) {
        xInicial=x=x0; yInicial=y=y0; w=w0; h=h0; color=c; panel=p;
    }
    /* Estos 3 mÃ©todos se utilizan para actualizar la posicion de los Graficos en el Cliente*/    
    public Integer getX(){
        return x;
    }
    
    public Integer getY(){
        return y;
    }
    
    public void setX(Integer x){
        this.x = x;
    }
    
    public void setY(Integer y){
        this.y = y;
    } 
    
    public Integer getW(){
        return w;
    }
    
    public Integer getH(){
        return h;
    } 
    
    public void setW(Integer w){
        this.w = w;
    }
    
    public void setH(Integer h){
        this.h = h;
    } 
            
    /* Este metodo se utiliza cuando se hace el repintado 
     de los Graficos contenidos en Figuras*/
    public abstract void dibujate(Graphics2D g2);
    
    /* Este metodo devuelve la forma necesaria para 
     verificar la intersecciÃ³n entre 2 Graficos*/
    /*protected Rectangle2D forma(){
        return new Rectangle( x, y, w, h);
    } */   
        
}