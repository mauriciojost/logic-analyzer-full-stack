package analizador;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Comunicador {
    byte[] bufferEntrada;
    
    public native void iniciar();
    public native void enviar(byte comando);
    public native byte recibir();
    
    static {
        System.loadLibrary("Comunicado");
    }
    
    public Comunicador(){
        this.iniciar();
    }
    
    public void enviarComando(String comando){
        int i;
        byte a;
        System.out.println("Enviando... ");
        for(i=0;i<comando.length();i++){
            
            a = (byte)comando.charAt(i);
            this.enviar(a);
            try {
                Thread.sleep(10);
            } catch (InterruptedException ex) {
                Logger.getLogger(Comunicador.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        this.enviar((byte)'\n');
    }
    
    public String recibirComando(){ /* Por el momento es la mentirosa... */
        char[] retorno = new char[500];
        boolean fin=false;
        byte recibido;
        int i=0;
        
        while(!fin){
            recibido = this.recibir();
            retorno[i++] = (char)recibido;
            if (recibido=='\n'){
                fin = true;
            }
        }
        
        return String.copyValueOf((char[])retorno, 0, i-1);
    }
    
}
