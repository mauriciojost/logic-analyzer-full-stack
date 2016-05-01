package analizador;

public interface Comunicable {
    
    public void enviarComando(String comando);
    
    /* Recibe comando del HW externo. */
    public String recibirComando();

    /* Retorna la ultima trama obtenida. */
    public String obtenerUltimaTrama();

    public void cerrarSerie();
    
    /* Salva la ultima trama recibida. */
    public void guardarMuestras();
    /* Carga un archivo de trama previamente guardado. */
    public void abrirMuestras();

    /* Crea un objeto de seleccion de archivos. */
    public void crearJFileChooser();

}







        

