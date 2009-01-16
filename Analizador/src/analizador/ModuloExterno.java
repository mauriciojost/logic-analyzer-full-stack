package analizador;

import java.util.Observable;
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
    private Parseador parseador;
    
    /* Constructor. */
    public ModuloExterno(Comunicador comunicador){
        this.comunicador = comunicador;
        ModuloExterno.moduloExterno = this;
        this.parseador = new Parseador();
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
                intento++; System.out.println("Intento de conexión nro. " + intento + ".");
                
                /* Envio del comando segun la solicitud del usuario. */
                comunicador.enviarComando("<i n=1 m="+ (modo?1:0) +" p="+periodous+"> </i>"); System.out.println("Comando enviado.");
                xml = comunicador.recibirComando(); System.out.println("Se recibio en JAVA: '" + xml + "'.");
        
                /* Parseo del XML recibido. */
                muestras = parseador.parseoMuestras(xml); crc_xml = parseador.parseoCRC(xml); crc_muestras = getCRC(); modo_xml = parseador.parseoModo(xml); periodous_xml = parseador.parseoPeriodo(xml);
                
                System.out.println("         Solicitado\t\tObtenido"); System.out.println("Modo:   " + this.modo + "\t\t" + modo_xml); System.out.println("CRC:    " + (int)crc_muestras + "\t\t" + (int)crc_xml); System.out.println("Periodo:" + (int)periodous + "\t\t" + (int)periodous_xml);

                if ((crc_muestras==crc_xml) && (modo==modo_xml) && (periodous==periodous_xml)){
                    this.notificarMuestras(muestras);
                    exitoso=true;
                }else{
                   throw new NullPointerException("Datos recibidos no validos.");
                }
                
                ControlMonitor.getControlMonitor().inicializar(); /* Inicializa al Control Monitor segun trama recibida. */
                
            }catch(NullPointerException e){
                System.out.println("Error al intentar conexión con el Módulo Externo de Hardware. No se ha reconocido la trama recibida."); exitoso=false;
            }catch(Exception e){
                e.printStackTrace(); exitoso=false;
            }
        }while((exitoso==false)&& (intento<CANTIDAD_DE_INTENTOS_MAXIMA));
        if (exitoso==false){
            JOptionPane.showMessageDialog(null, "No se ha podido establecer conexión con el puerto serie.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public void cargarArchivo(){
        String xml;
        comunicador.abrirMuestras();
        xml = comunicador.obtenerUltimaTrama();
        muestras = parseador.parseoMuestras(xml);
        this.notificarMuestras(muestras);
    }

    public void cambiarModo(boolean modo){
        this.modo = modo;
    }
    
    public void cambiarFreqHz(long freqHz){
        this.freqHz = freqHz;
    }
    private void notificarMuestras(char[] muestras){
        this.setChanged();
        this.notifyObservers(muestras);
    }
    
    private char getCRC(){
        char crc=0; int i;
        for (i=0;i<muestras.length;i++)
            crc = (char)((crc + muestras[i]) % 256);
        return crc;
    }
}
