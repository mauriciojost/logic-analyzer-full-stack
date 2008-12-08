
package analizador;

import java.util.Observable;

public class ModuloExterno extends Observable{
    private static ModuloExterno moduloExterno;
    private int estado;
    private boolean modo;
    private int veloc;
    private char[] señal;
    private char[] muestras;
    private Comunicador comunicador;
    
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
        System.out.println("Escribió: " + comunicador.recibirComando());
    }

    private char[] generarSeñalAleatoria(){ // Sólo a los efectos de la depuración.
        char[] rango = new char[1024];
        for (int k=0;k<1024;k++){
            rango[k] = (char)(k);
        }
        return rango;
    }

    public int obtenerEstado(){
        return 0;
    }
    
    private char[] parseo_choto(String xml){ /*Esto lo tenés que tocar vos Guille!!! */
        String string_cabecera = "<inicio nuevo=1 modo=1 velocidad=1000>";
        int indice = xml.indexOf(string_cabecera);
        int entero=0;
        int i=0;
        char[] retorno = new char[1024];
        String string = "";
        boolean sigue=true;
        
        indice = indice + string_cabecera.length();
        System.out.println("El primer valor luego de la cabecera es ("+indice+"): " + xml.substring(indice));
        xml = xml.substring(indice); // Descarta la cabecera.
        
        
        while (sigue){
            xml = xml.trim(); // Quita espacios iniciales y finales.
            indice = xml.indexOf(" "); // Se fija el primer espacio ("11 22 33 44 ..."), que delimita el "11" (valor a analizar).
            string = xml.substring(0,indice); // Pone en string el "11".
            try{
                entero = Integer.valueOf(string); // Pone en entero un 11.
                sigue = true; // Indica que este valor fue válido, buscar más...
            }catch(Exception e){
                sigue = false; // Dejar de buscar, puesto que este valor fue inválido.
            }
            if (sigue){
                retorno[i] = (char)entero;
                i++;
            }
            System.out.println(entero);
            xml = xml.substring(indice); // Se va descartando lo que se analizó.
        }
        
        return retorno;
    }
    
    public void iniciarMuestreo(){
        String xml = comunicador.recibirComando();
        /* En esta parte viene el parseo para extraer los datos.*/
        /* En esta parte viene el CRC para verificar la validez de los datos obtenidos. */
        
        señal = parseo_choto(xml); /* Cosa a tocar por vos Guille!!! */
        
        this.setChanged();
        this.notifyObservers(señal);
    }
    
    public void cambiarModo(boolean modo){
        
    }
    
    public void cambiarVelocidad(int veloc){
        
    }
    private void notificarMuestras(char[] muestras){
        
    }

    public static ModuloExterno getModuloExterno(){
        return moduloExterno;
    }
}
