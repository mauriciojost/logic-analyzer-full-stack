
package analizador;

import java.util.Observable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ModuloExterno extends Observable{
    private static ModuloExterno moduloExterno;
    private int estado;
    private boolean modo;
    private int veloc;
    private int crc;
    private char[] muestras;
    private Comunicador comunicador;
    
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
        System.out.println("Escribió: " + comunicador.recibirComando());
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
            //System.out.println(entero);
            xml = xml.substring(indice); // Se va descartando lo que se analizó.
        }
        
        return retorno;
    }
    
    public void iniciarMuestreo(){
        String xml;
        
        comunicador.enviarComando("<inicio nuevo=1 modo="+ (modo?1:0) +" velocidad="+veloc+"> </inicio>"); // Correcto.
        //comunicador.enviarComando("<iinicio nuevo=1 modo=1 velocidad=1000> </inicio>"); // Inválido.
        
        xml = comunicador.recibirComando();
        /* En esta parte viene el parseo para extraer los datos. -->> GUILLE!
         * Recordar que la extracción de datos, no sólo implica las muestras, 
         * sino todo un estado (modo, velocidad, etc.). 
         * En esta parte viene el CRC para verificar la validez de los datos obtenidos. 
         */
        
        muestras = parseo_choto(xml); /* Cosa a tocar por vos Guille!!! No olvidar el estado! */
        parseoMuestras(xml);
        crc = parseoCRC(xml);
        modo = parseoModo(xml);
        veloc = parseoVelocidad(xml);
        // inicio = parseo...
        
        
        
        // if crc valido, inicio=requerido, modo=requerido,velocidad=requerida    
        this.notificarMuestras(muestras);
        // else
        //   mostrar error
        // end if
    }
    
    public void cambiarModo(boolean modo){
        this.modo = modo;
    }
    
    public void cambiarVelocidad(int veloc){
        this.veloc = veloc;
    }
    private void notificarMuestras(char[] muestras){
        this.setChanged();
        this.notifyObservers(muestras);
    }

    public static ModuloExterno getModuloExterno(){
        return moduloExterno;
    }
    
    private char getCRC(char[] muestras){
        char crc=0;
        int i;
        for (i=0;i<muestras.length;i++){
            crc = muestras[i];
        }
        return crc;
    }

    public boolean parseoModo(String s){
        boolean mode = false;
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d modo=(\\d) velocidad=\\d+>");
        Matcher m = strMatch.matcher(s);
        while (m.find()){
            //System.out.println( "string = " + m.group(0) );
            System.out.println( "modo = " + m.group(1));
            mode = (m.group(1).equals("1"));
        }
        return mode;
    }

    public int parseoVelocidad(String s){
        int velo = 0;
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d modo=\\d velocidad=(\\d+)>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            //System.out.println( "string = " + m.group(0) );
            System.out.println( "velocidad = " + m.group(1) );
            velo = Integer.valueOf(m.group(1));
        }
        return velo;
    }

    public int parseoCRC(String s){
        int CRC = 0;
        Pattern strMatch = Pattern.compile( "<CRC> (\\w+) </CRC>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            //System.out.println( "string = " + m.group(0) );
            System.out.println( "CRC = " + m.group(1) );
            CRC = Integer.valueOf(m.group(1));
        }
        return CRC;
    }

    public char[] parseoMuestras(String s){
        String muestrasTemp = null;
        char muestrasRet[] = null;
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d modo=\\d velocidad=\\d+> (.*) <CRC> \\w+ </CRC> </inicio>");
        Matcher m = strMatch.matcher(s);
        while ( m.find() ){
            muestrasTemp = m.group(1);
        }
        System.out.println(muestrasTemp);

        /* Falta la conversion de String a array de chars creeme
         * que trate de hacerla durante varias horas pero no pude
         * lograr algo que ande. 
         * Tambien descubri que no se le puede exigir mucho al regex,
         * poniendole una expresion regular detallada no soporta mas de
         * 750 muestras y larga muchisimas exceptiones.
         */

        return muestrasRet;
    }

}
