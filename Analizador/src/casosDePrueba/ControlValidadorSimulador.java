
package casosDePrueba;

import analizador.*;

/* Clase Control Validador
 * Verifica que los datos entregados por el usuario sean validos.
 * Es el nexo entre ModuloExterno y el usuario.
 */
public class ControlValidadorSimulador {
    private boolean parametros_validos=false;
    private ModuloExterno moduloExterno;
    
    public static void main(String[] args) {
        ControlMonitor contr = new ControlMonitor(); 
        /* ^ nenesario para evitar NullPointerException en ModuloExterno*/
        new ControlValidadorSimulador(new ModuloExterno(Comunicador.newComunicador()));
    }
    
    public ControlValidadorSimulador(ModuloExterno moduloExterno){
        this.moduloExterno = moduloExterno;
        pruebaDeInicioDeMuestreo();
    }
    
    private void pruebaDeInicioDeMuestreo(){
        try{
            inicializarModuloExterno(true,1000);
            solicitarInicioMuestreo();
        }catch(Exception e){
            e.printStackTrace();
        }
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
