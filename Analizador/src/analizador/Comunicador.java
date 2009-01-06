package analizador;


public class Comunicador {
    byte[] bufferEntrada;
    
    public native void iniciar();
    public native void enviar(byte comando);
    public native String recibir();
    
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
            try {Thread.sleep(1);} catch (InterruptedException ex) {ex.printStackTrace();} // Espera para no atorar al PIC.
        }
        this.enviar((byte)'\n');
    }
    
    public String recibirComando(){ /* Por el momento es la mentirosa... */
        return this.recibir();
    }
    
}
