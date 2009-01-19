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
 
    private Comunicable comunicador;
    
    public static void main(String[] args) {
        new ModuloExternoSimulador(Comunicador.newComunicador());
     }
    
    /* Constructor. */
    public ModuloExternoSimulador(Comunicable comunicador){
        this.comunicador = comunicador;
        pruebaDeEnvioDeComando();        
    }

    private void pruebaDeEnvioDeComando(){
        comunicador.enviarComando("<i n=4 m=1 p=100> </i>");
        System.out.println("Comando enviado.");
    }
}