package analizador;

import java.io.*;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;

/*
 * Clase Comunicador. 
 * Enlace entre la abstraccion ModuloExterno y el HW externo.
 */
public class Comunicador implements Comunicable{
    byte[] ultimaTramaRecibida;
    private JFileChooser fc;
    private boolean conectado=false;
    
    /* Metodos de nivel inferior. JNI. */
    private native int iniciar();
    private native void enviar(byte comando);
    private native String recibir();
    private native void finalizar();
    
    /* Carga de la libreria: JNI. */
    static {
        System.out.println("Comunicador. Buscando librerias en:" + System.getProperty("java.library.path"));

        //String strliblin = "/usr/lib/incomunicado.so"; // Simulacion de HW.
        //String strlibwin = "Incomunicado";

        String strliblin = "/usr/lib/comunicado.so"; // Uso de HW real.
        String strlibwin = "Comunicado";

        try{
            System.load(strliblin);
            System.out.println("Librería de Linux cargada correctamente.");
        }catch(Throwable e){
            System.out.println("Error al cargar la librería de Linux('"+strliblin+"').");
            System.out.println("Put the corrresponding file in Libs (.dll or .so) into the OS libraries directory.");
            System.out.println("For Windows usually it is '#WINDOWS#System32' and for Linux '/usr/lib/'.");
            //e.printStackTrace();
            try{
                System.loadLibrary(strlibwin);
                System.out.println("Librería de Windows cargada correctamente.");
            }catch(Throwable b){
                System.out.println("Error al cargar la librería de Windows('"+strlibwin+".dll').");
                //b.printStackTrace();
            }
            
        }
        
    }
    
    /* Metodo constructor. */
    public Comunicador(){
        crearJFileChooser();
        int fd = this.iniciar();
        if (fd==-1){
            System.out.println("No se ha podido abrir el puerto serie.");
            JOptionPane.showMessageDialog(null, "No se ha podido abrir el puerto serie.", "Error", JOptionPane.ERROR_MESSAGE);
            conectado=false;
        }else{
            conectado=true;
        }
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
    public void guardarMuestras(){
        File file = new File (fc.getCurrentDirectory()+"\\Signal.sgl");
        fc.setSelectedFile(file);
        int returnVal = fc.showSaveDialog(null);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            try {
                FileOutputStream fos = new FileOutputStream(fc.getSelectedFile());
                fos.write(ultimaTramaRecibida);
                fos.close();
            } catch (FileNotFoundException ex) {
                System.out.println("Archivo no encontrado: " + ex);
            } catch (IOException ex) {
                System.out.println("Error IO: " + ex);
            }
            System.out.println("Salvando: " + fc.getSelectedFile().getAbsolutePath());
        } else {
            System.out.println("Cancelado por el usuario.");
        }
    }

    /* Carga un archivo de trama previamente guardado. */
    public void abrirMuestras(){
        fc.setCurrentDirectory(null);
        int returnVal = fc.showOpenDialog(null);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            try {
                FileInputStream fis = new FileInputStream(fc.getSelectedFile());
                BufferedInputStream buf = new BufferedInputStream(fis);
                ultimaTramaRecibida = new byte[buf.available()];
                fis.read(ultimaTramaRecibida);
                fis.close();
                  /* Es necesario hacer un metodo en ModuloExterno
                 similar a iniciarMuestreo() que parsee bufferEntrada
                 y que notifique las muestras para que se dibujen */
                System.out.println("Abriendo: " + fc.getSelectedFile().getAbsolutePath());
                String str = new String(ultimaTramaRecibida);
                System.out.println(new String(ultimaTramaRecibida));
            } catch (FileNotFoundException ex) {
                System.out.println("Archivo no encontrado: " + ex);
            } catch (IOException ex) {
                System.out.println("Error IO: " + ex);
            }
        } else {
            System.out.println("Cancelado por el usuario.");
        }
    }

    /* Crea un objeto de seleccion de archivos. */
    public void crearJFileChooser(){
        fc = new JFileChooser(){
            @Override
            public void approveSelection() {
                File f = getSelectedFile();
                if(f.exists() && getDialogType() == SAVE_DIALOG) {
                    int result = JOptionPane.showConfirmDialog(getTopLevelAncestor(),
                            "El archivo seleccionado ya existe. " +
                            "¿Desea sobreescribirlo?",
                            "El archivo ya existe",
                            JOptionPane.YES_NO_CANCEL_OPTION,
                            JOptionPane.QUESTION_MESSAGE);
                    switch(result)  {
                    case JOptionPane.YES_OPTION:
                        super.approveSelection();
                        return;
                    case JOptionPane.NO_OPTION:
                        return;
                    case JOptionPane.CANCEL_OPTION:
                        cancelSelection();
                        return;
                    }
                }
                super.approveSelection();
            }
        };

        FileFilter filter = new FileFilter(){
            public boolean accept(File f) {
                if (f.isDirectory()) return true;
                return f.getAbsolutePath().endsWith("sgl");
            }
            @Override
            public String getDescription() {
                 return "Señales";
            }
        };
        fc.setFileFilter(filter);
    }

}
