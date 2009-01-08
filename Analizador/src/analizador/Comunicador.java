package analizador;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.filechooser.FileFilter;

public class Comunicador {
    byte[] bufferEntrada;
    private JFileChooser fc;
    
    public native void iniciar();
    public native void enviar(byte comando);
    public native String recibir();
    
    static {
        System.loadLibrary("Comunicado");
    }
    
    public Comunicador(){
        crearJFileChooser();
        this.iniciar();
    }
    
    public void enviarComando(String comando){
        int i;
        byte a;
        System.out.println("Enviando comando: '"+ comando + "'...");
        for(i=0;i<comando.length();i++){
            
            a = (byte)comando.charAt(i);
            this.enviar(a);
            try {Thread.sleep(1);} catch (InterruptedException ex) {ex.printStackTrace();} // Espera para no atorar al PIC.
        }
        this.enviar((byte)13);
    }
    
    public String recibirComando(){ /* Por el momento es la mentirosa... */
        String retorno = this.recibir();
        bufferEntrada = retorno.getBytes();
        return retorno;
    }

    public String obtenerComando(){
        String retorno = new String(bufferEntrada);
        return retorno;
    }

    public void guardarMuestras(){
        File file = new File (fc.getCurrentDirectory()+"\\Signal.sgl");
        fc.setSelectedFile(file);
        int returnVal = fc.showSaveDialog(null);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            try {
                FileOutputStream fos = new FileOutputStream(fc.getSelectedFile());
                fos.write(bufferEntrada);
                fos.close();
            } catch (FileNotFoundException ex) {
                System.out.println("FileNotFoundException : " + ex);
            } catch (IOException ex) {
                System.out.println("IOException : " + ex);
            }
            System.out.println("Saving: " + fc.getSelectedFile().getAbsolutePath());
        } else {
            System.out.println("Save command cancelled by user.");
        }
    }

    public void abrirMuestras(){
        fc.setCurrentDirectory(null);
        int returnVal = fc.showOpenDialog(null);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            try {
                FileInputStream fis = new FileInputStream(fc.getSelectedFile());
                BufferedInputStream buf = new BufferedInputStream(fis);
                bufferEntrada = new byte[buf.available()];
                fis.read(bufferEntrada);
                fis.close();
                  /* Es necesario hacer un metodo en ModuloExterno
                 similar a iniciarMuestreo() que parsee bufferEntrada
                 y que notifique las muestras para que se dibujen */
                System.out.println("Opening: " + fc.getSelectedFile().getAbsolutePath());
                String str = new String(bufferEntrada);
                System.out.println(new String(bufferEntrada));
            } catch (FileNotFoundException ex) {
                System.out.println("FileNotFoundException : " + ex);
            } catch (IOException ex) {
                System.out.println("IOException : " + ex);
            }
        } else {
            System.out.println("Open command cancelled by user.");
        }
    }

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
