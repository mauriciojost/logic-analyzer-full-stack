
package analizador;

public class Comunicador {
    
    byte[] bufferEntrada;
    
    int chanchada;
    
    public Comunicador(){
        
    }
    
    public char enviarComando(String comando){
        if (comando.startsWith("<inicio") && comando.endsWith("/inicio>")){
            chanchada = 11;
            return '1';
        }else{
            chanchada = 22;
            return '0';
        }   
    }
    
    public String recibirComando(){ /* Por el momento es la mentirosa... */
        String retorno = "<inicio nuevo=1 modo=1 velocidad=1000> ";
        //Random a = new Random();
        int i;
        if (chanchada == 11){
            for (i=0;i<1024;i++){
                int entero = i;//a.nextInt(255);
                String string = Integer.valueOf(entero).toString();
                retorno = retorno.concat(string + " ");
                
            }
            retorno = retorno.concat("<CRC> 32423 </CRC> </inicio>");
            return retorno;
        }else{
            return "";
        }
    }
}
