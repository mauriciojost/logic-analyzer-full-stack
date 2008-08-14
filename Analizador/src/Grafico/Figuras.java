package Grafico;

import java.awt.Graphics2D;
import java.util.Vector;

public class Figuras extends Grafico{
    Vector <Grafico>figuras = new Vector<Grafico>();
    static Figuras f = null;

    public Figuras () {
        super(null,null,null,null,null,null);
        f  = this;
    }

    public static Figuras getInstance(){        
        return f ;
    }

    public int size(){
        return figuras.size();
    }

    public Grafico get(int i) {
        return figuras.get(i);
    }

    public int indexOf(Grafico x){
        return figuras.indexOf(x);
    }

    public void add(Grafico g2) {
        figuras.add(g2);
    }
    
    /* Todavia no fue usado nunca!!!!!!!!!!!!!!! */
    public void remove(Integer i) {
        figuras.remove(i);
    }    
    
    /* Este metodo se usa para dibujar todas las figuras */
    @Override
    public void dibujate(Graphics2D g2) {
        for(int i=0; i<size(); i++) {get(i).dibujate(g2);} 
    }

    public void ZoomInX(Graphics2D g2){
        for(int i=0; i<size(); i++) {
            if(get(i).getClass().getSimpleName().startsWith("LineaHorizontal"))
                get(i).setW(get(i).getW()+10);
            get(i).setX(get(i).getX()+ i * 10); 
            get(i).dibujate(g2);            
        } 
        //for hasta recorrer todo las figuras
        //sacar un grafico del array
        //si es Horizontal aumentar su longitud en X pixeles 
        //Desplazar el grafico (V u H) con la siguiente regla: 
        //setX(getX()+ indexOf(grafico) * X) 
    }
    
    public void ZoomOutX(Graphics2D g2){
        for(int i=0; i<size(); i++) {
            if(get(i).getClass().getSimpleName().startsWith("LineaHorizontal"))
                get(i).setW(get(i).getW()-10);
            get(i).setX(get(i).getX()- i * 10); 
            get(i).dibujate(g2);            
        } 
        //for hasta recorrer todo las figuras
        //sacar un grafico del array
        //si es Horizontal disminuir su longitud en X pixeles 
        //Desplazar el grafico (V u H) con la siguiente regla: 
        //setX(getX() - indexOf(grafico) * X) 
    }
}
