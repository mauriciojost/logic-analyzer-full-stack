package casosDePrueba;

import analizador.*;

/* Clase ModuloExterno.
 * Representacion de alto nivel del Hardware externo.
 * Es objeto observado (patron Observer).
 */
/* Clase ModuloExterno.
 * Representacion de alto nivel del Hardware externo.
 * Es objeto observado (patron Observer).
 */
public class ModuloExternoSimulador {
 
    private Comunicador comunicador;
    
    public static void main(String[] args) {
        new ModuloExternoSimulador(new Comunicador());
     }
    
    /* Constructor. */
    public ModuloExternoSimulador(Comunicador comunicador){
        this.comunicador = comunicador;
        pruebaDeEnvioDeComando();        
    }

    private void pruebaDeEnvioDeComando(){
        comunicador.enviarComando("ESTADO");
        System.out.println("Comando enviado.");
    }
}