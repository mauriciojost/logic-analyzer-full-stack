package analizador;

import java.util.Observable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ModuloExterno extends Observable{
    private static ModuloExterno moduloExterno;
    private int estado;
    private boolean modo;
    private int veloc;
    
    private char[] muestras;
    private Comunicador comunicador;
    
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
    }

    public int obtenerEstado(){
        return 0;
    }
    
    public void iniciarMuestreo(){
        String xml;
        boolean modo_xml;
        int veloc_xml;
        int crc_xml;
       
        comunicador.enviarComando("<inicio nuevo=1 modo="+ (modo?1:0) +" velocidad="+veloc+"> </inicio>"); // Correcto.
        //comunicador.enviarComando("<iinicio nuevo=1 modo=1 velocidad=1000> </inicio>"); // Inválido.
        
        System.out.println("\nComando enviado...");
        xml = comunicador.recibirComando();
        System.out.println("Se recibio en JAVA: '" + xml + "'.");
        
        muestras = parseoMuestras(xml);
        crc_xml = parseoCRC(xml);
        modo_xml = parseoModo(xml);
        veloc_xml = parseoVelocidad(xml);
        
        
        
        
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
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d+ modo=(\\d+) velocidad=\\d+>");
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
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d+ modo=\\d+ velocidad=(\\d+)>");
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
        int indice=0, entero=0, i=0;
        char[] retorno = new char[1024];
        
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d+ modo=\\d+ velocidad=\\d+> (.*) <CRC> \\w+ </CRC> </inicio>");
        Matcher m = strMatch.matcher(s);
        
        while ( m.find() ){
            muestrasTemp = m.group(1).trim();
        }

        while (indice!=-1){
            indice = muestrasTemp.indexOf(" ");     // Se fija el primer espacio ("11 22 33 44 ..."), que delimita el "11" (valor a analizar).
            if (indice!=-1){                        // Se ha encontrado espacio?
                //int i= Integer.parseInt(str,16); EN HEXADECIMAL!!!
                entero = Integer.valueOf(muestrasTemp.substring(0, indice));    // Pone en entero un 11.
                muestrasTemp = muestrasTemp.substring(indice).trim();           // Descartar parte ya analizada.
            }else{   
                entero = Integer.valueOf(muestrasTemp);                         // Último valor (no termina con espacio).
            }
            retorno[i++] = (char)entero;            // Asignar valor obtenido al vector.
        }
        
        char[] retorno_justo = new char[i];
        for(i=0;i<retorno_justo.length;i++){retorno_justo[i]=retorno[i];}
        
        return retorno_justo;
    }

}
