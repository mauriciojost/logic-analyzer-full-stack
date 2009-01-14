package analizador;

import java.util.Observable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.JOptionPane;

/* Clase ModuloExterno.
 * Representacion de alto nivel del Hardware externo.
 * Es objeto observado (patron Observer).
 */
public class ModuloExterno extends Observable{
    private int CANTIDAD_DE_INTENTOS_MAXIMA = 3;
    
    private static ModuloExterno moduloExterno;             
    private int estado;                                 /* Estado del ModuloExterno.                        */
    private boolean modo=false;                         /* Modo de muestreo solicitado: False -> Sincrono.  */
    private long freqHz;                                /* Frecuencia (en Hz) solicitada.                   */
    private char[] muestras;                            /* Conjunto de muestras obtenidas.                  */
    private Comunicador comunicador;                    
    
    /* Constructor. */
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
    }

    /* Retorna el estado del ModuloExterno. */
    public int obtenerEstado(){
        return 0;
    }
    
    /* Inicia un muestreo. */
    public void iniciarMuestreo(){
        String xml;                             /* XML retornado por el HW externo: respuesta a la solicitud.   */
        boolean modo_xml;                       /* Modo obtenido a partir del xml retornado por el HW externo.  */
        long periodous, periodous_xml;          /* Periodos de usuario, y retornado por el HW externo.          */
        char crc_muestras, crc_xml;             /* Suma de verificacion de usuario (solicitada) y de HW externo.*/
        
        periodous = Math.round(((double)(1000000.0/freqHz)));
        
        boolean exitoso=false;                  /* Indicador de conexion exitosa.                               */
        int intento=0;                          /* Numero de intento.                                           */
        do{
            try{
                intento++;
                System.out.println("Intento de conexión nro. " + intento + ".");
                comunicador.enviarComando("<inicio nuevo=1 modo="+ (modo?1:0) +" velocidad="+periodous+"> </inicio>"); // Correcto.
                
                System.out.println("Comando enviado.");
                xml = comunicador.recibirComando();
        
                System.out.println("Se recibio en JAVA: '" + xml + "'.");
        
                muestras = parseoMuestras(xml);
                crc_xml = parseoCRC(xml);
                crc_muestras = getCRC();
                modo_xml = parseoModo(xml);
                periodous_xml = parseoPeriodo(xml);
                
                System.out.println("         Solicitado\t\tObtenido");
                System.out.println("Modo:   " + this.modo + "\t\t" + modo_xml);
                System.out.println("CRC:    " + (int)crc_muestras + "\t\t" + (int)crc_xml);
                System.out.println("Periodo:" + (int)periodous + "\t\t" + (int)periodous_xml);

                if ((crc_muestras==crc_xml) && (modo==modo_xml) && (periodous==periodous_xml)){
                    this.notificarMuestras(muestras);
                    exitoso=true;
                }else{
                   throw new NullPointerException("Datos recibidos no validos.");
                }
                
                ControlMonitor.getControlMonitor().inicializar();
                
            }catch(NullPointerException e){
                System.out.println("Error al intentar conexión con el Módulo Externo de Hardware. No se ha reconocido la trama recibida.");
                exitoso=false;
            }catch(Exception e){
                e.printStackTrace();
                exitoso=false;
            }
        }while((exitoso==false)&& (intento<CANTIDAD_DE_INTENTOS_MAXIMA));
        if (exitoso==false){
            JOptionPane.showMessageDialog(null, "No se ha podido establecer conexión con el puerto serie.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public void cargarArchivo(){
        String xml;
        boolean modo_xml;
        int veloc_xml;
        int crc_xml;
        long periodous;
        double intermedioSEG;


        comunicador.abrirMuestras();
        xml = comunicador.obtenerComando();

        muestras = parseoMuestras(xml);
        crc_xml = parseoCRC(xml);
        modo_xml = parseoModo(xml);
        veloc_xml = parseoPeriodo(xml);

        this.notificarMuestras(muestras);
    }

    public void cambiarModo(boolean modo){
        this.modo = modo;
    }
    
    public void cambiarFreqHz(int freqHz){
        this.freqHz = freqHz;
    }
    private void notificarMuestras(char[] muestras){
        this.setChanged();
        this.notifyObservers(muestras);
    }

    public static ModuloExterno getModuloExterno(){
        return moduloExterno;
    }
    
    private char getCRC(){
        char crc=0;
        int i;
        for (i=0;i<muestras.length;i++){
            crc = (char)((crc + muestras[i]) % 256);
        }
        return crc;
    }

    public boolean parseoModo(String s){
        boolean mode = false;
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d+ modo=(\\d+) velocidad=\\d+>");
        Matcher m = strMatch.matcher(s);
        while (m.find()){
            mode = (m.group(1).equals("1"));
        }
        return mode;
    }

    public int parseoPeriodo(String s){
        int velo = 0;
        Pattern strMatch = Pattern.compile( "\\<inicio nuevo=\\d+ modo=\\d+ velocidad=(\\d+)>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            velo = Integer.valueOf(m.group(1));
        }
        return velo;
    }

    public char parseoCRC(String s){
        int CRC = 0;
        Pattern strMatch = Pattern.compile( "<CRC> (\\w+) </CRC>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            CRC = Integer.valueOf(m.group(1));
        }
        return (char)CRC;
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
