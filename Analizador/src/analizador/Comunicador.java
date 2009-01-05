package analizador;

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
        System.out.println("Enviando... ");
        for(i=0;i<comando.length();i++){
            this.enviar((byte)comando.charAt(i));
        }
        this.enviar((byte)'\n');
    }
    
    public String recibirComando(){ /* Por el momento es la mentirosa... */
        char[] retorno = new char[20000];
        boolean fin=false;
        byte recibido;
        int i=0;
        
        while(!fin){
            recibido = this.recibir();
            retorno[i++] = (char)recibido;
            String.valueOf(recibido);
            if (recibido=='\n'){
                fin = true;
            }
        }
        
        return String.copyValueOf((char[])retorno, 0, i-1);
    }
    
}
