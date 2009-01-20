
package casosDePrueba;

import analizador.*;


public class ControlValidadorSimulador {
    
    private ModuloExterno moduloExterno;
    
    public static void main(String[] args) {
        ControlMonitor contr = new ControlMonitor(); 
        /* ^ nenesario para evitar NullPointerException en ModuloExterno*/
        ModuloExterno moduEx = new ModuloExterno(Comunicador.newComunicador());
        ControlValidadorSimulador control_validador = new ControlValidadorSimulador(moduEx);
        
        CanalSimulador canSim = new CanalSimulador(0); 
        moduEx.addObserver(canSim);
        
        
        for(int j=0;j<25;j++){
            try{Thread.sleep(1000);}catch(Exception e){e.printStackTrace();}
            control_validador.pruebaDeInicioDeMuestreo();
        }
    }
    
    public ControlValidadorSimulador(ModuloExterno moduloExterno){
        this.moduloExterno = moduloExterno;
    }
    
    public void pruebaDeInicioDeMuestreo(){
        try{
            inicializarModuloExterno(true,1000);
            solicitarInicioMuestreo();
        }catch(Exception e){
            e.printStackTrace();
        }
    }
    
    /* Inicializa a ModuloExterno.                                              */
    public void solicitarInicioMuestreo() throws Exception{
        this.moduloExterno.iniciarMuestreo();
    }
    
    public void inicializarModuloExterno(boolean modo, long freqHz){
        try {
            this.moduloExterno.cambiarFreqHz(freqHz);
            this.moduloExterno.cambiarModo(modo);
        
        } catch (Exception ex) {
            ex.printStackTrace();
        }   
    }

    
}
