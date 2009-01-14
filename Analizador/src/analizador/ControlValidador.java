
package analizador;

public class ControlValidador {
    private boolean parametros_validos=true;
    private boolean modo;
    private int veloc;
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
    
    public void solicitarCambioModo(boolean modo){
        this.moduloExterno.cambiarModo(modo);
    }
    
    public void solicitarCambioFreqHz(long freqHz) throws Exception{
        if ((freqHz>500) && (freqHz<= 1000000)){
            this.moduloExterno.cambiarFreqHz(freqHz);
            parametros_validos=true;
        }else{
            parametros_validos=false;
            throw new Exception("Frecuencia no valida. ");
        }
    }
}
