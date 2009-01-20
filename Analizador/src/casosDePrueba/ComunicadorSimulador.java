package casosDePrueba;

import analizador.*;

class ComunicadorSimulador implements Comunicable{

    byte[] ultimaTramaRecibida;
    private boolean conectado=false;
    
    /* Metodos de nivel inferior. JNI. */
    private native int iniciar();
    private native void enviar(byte comando);
    private native String recibir();
    private native void finalizar();
    
    /* Carga de la libreria: JNI. */
    static {
        System.out.println("Buscando librerias en:" + System.getProperty("java.library.path"));
        try{
            System.load("/usr/lib/comunicado.so");
            System.out.println("Librería de Linux cargada correctamente.");
        }catch(java.lang.UnsatisfiedLinkError e){
            System.out.println("Error al cargar la librería en Linux.");
            e.printStackTrace();
            try{
                System.loadLibrary("Comunicado");
                System.out.println("Librería de Windows cargada correctamente.");
            }catch(Exception b){
                System.out.println("Error al cargar la librería en Windows.");
                b.printStackTrace();
            }
            
        }
        
    }
    
    /* Metodo constructor. */
    private ComunicadorSimulador(){
        int fd = this.iniciar();
        if (fd==-1){
            System.out.println("No se ha podido abrir el puerto serie.");
            conectado=false;
        }else{
            conectado=true;
        }
    }
    
    /* Metodo constructor. */
    public static Comunicable newComunicador(){
        return new ComunicadorSimulador();
    }
    
    /* Envia comando hacia el HW externo. */
    public void enviarComando(String comando){
        int i;
        byte a;
        if (conectado){
            System.out.println("Enviando comando: '"+ comando + "'...");
            this.enviar((byte)'\n');              /* Indicador de flush. */
            try {                           /* Espera para el HW externo.           */
                    Thread.sleep(5);} catch (InterruptedException ex) {ex.printStackTrace();} 
            
            for(i=0;i<comando.length();i++){    /*Envio byte por byte.                  */
                a = (byte)comando.charAt(i);
                this.enviar(a);
                try {                           /* Espera para el HW externo.           */
                    Thread.sleep(2);} catch (InterruptedException ex) {ex.printStackTrace();} 
            }
            this.enviar((byte)'\r');              /* Indicador de nueva trama.            */
        }else{
            System.out.println("No se ha establecido conexión aún.");
        }
    }
    
    /* Recibe comando del HW externo. */
    public String recibirComando(){
        String retorno;   
        if (conectado) {
            System.out.println("Recibiendo...");
            retorno = this.recibir();           /* Llamada JNI.                         */
            ultimaTramaRecibida = retorno.getBytes();
        }else{
           throw new NullPointerException("Hardware externo no conectado...");
        }
        return retorno;
    }

    /* Retorna la ultima trama obtenida. */
    public String obtenerUltimaTrama(){
        String retorno = new String(ultimaTramaRecibida);
        return retorno;
    }

    public void cerrarSerie(){
        this.finalizar();
    }
    
    /* Salva la ultima trama recibida. */
    public void guardarMuestras(){}

    /* Carga un archivo de trama previamente guardado. */
    public void abrirMuestras(){}

    /* Crea un objeto de seleccion de archivos. */
    public void crearJFileChooser(){}    
}

