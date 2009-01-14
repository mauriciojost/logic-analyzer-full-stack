
package analizador;

public class ControlValidador {
    private boolean parametros_validos=false;
    private ModuloExterno moduloExterno;
    
    public ControlValidador(ModuloExterno moduloExterno){
        this.moduloExterno = moduloExterno;
    }
    
    /* Inicializa a ModuloExterno.                                              */
    public void solicitarInicioMuestreo() throws Exception{
        if (parametros_validos){
            this.moduloExterno.iniciarMuestreo();
        }else{
            throw new Exception("Parametros cargados no validos.");
        }
    }
    
    public void inicializarModuloExterno(boolean modo, long freqHz){
        try {
            this.solicitarCambioFreqHz(freqHz);
            this.solicitarCambioModo(modo);
            parametros_validos=true;
        } catch (Exception ex) {
            ex.printStackTrace();
        }   
    }
    
    public void solicitarCambioModo(boolean modo){
        this.moduloExterno.cambiarModo(modo);
    }
    
    public void solicitarCambioFreqHz(long freqHz) throws Exception{
        if ((freqHz>500) && (freqHz<= 1000000)){
            parametros_validos=true;
            this.moduloExterno.cambiarFreqHz(freqHz);
        }else{
            parametros_validos=false;
            throw new Exception("Frecuencia no valida. ");
        }
    }
}
