
package analizador;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parseador {

    public boolean parseoModo(String s){
        boolean mode = false;
        Pattern strMatch = Pattern.compile( "\\<i n=\\d+ m=(\\d+) p=\\d+>");
        Matcher m = strMatch.matcher(s);
        while (m.find()){
            mode = (m.group(1).equals("1"));
        }
        return mode;
    }

    public int parseoPeriodo(String s){
        int velo = 0;
        Pattern strMatch = Pattern.compile( "\\<i n=\\d+ m=\\d+ p=(\\d+)>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            velo = Integer.valueOf(m.group(1));
        }
        return velo;
    }

    public char parseoCRC(String s){
        int CRC = 0;
        Pattern strMatch = Pattern.compile( "<v> (\\w+) </v>");
        Matcher m = strMatch.matcher( s );
        while ( m.find() ){
            CRC = Integer.valueOf(m.group(1));
        }
        return (char)CRC;
    }

    
    public char[] parseoMuestras(String s){
        String muestrasTemp = null;
        int indice=0, entero=0, i=0;
        char[] retorno = new char[1024];
        
        Pattern strMatch = Pattern.compile( "\\<i n=\\d+ m=\\d+ p=\\d+> (.*) <v> \\w+ </v> </i>");
        Matcher m = strMatch.matcher(s);
        
        while ( m.find() ){
            muestrasTemp = m.group(1).trim();
        }

        while (indice!=-1){
            indice = muestrasTemp.indexOf(" ");     // Se fija el primer espacio ("11 22 33 44 ..."), que delimita el "11" (valor a analizar).
            if (indice!=-1){                        // Se ha encontrado espacio?
                entero = Integer.valueOf(muestrasTemp.substring(0,indice).trim(),16).intValue(); //EN HEXADECIMAL!!!
                //entero = Integer.valueOf(muestrasTemp.substring(0, indice));    // Pone en entero un 11.
                muestrasTemp = muestrasTemp.substring(indice).trim();           // Descartar parte ya analizada.
            }else{   
                entero = Integer.valueOf(muestrasTemp.trim(),16).intValue();                         // Último valor (no termina con espacio).
            }
            retorno[i++] = (char)entero;            // Asignar valor obtenido al vector.
        }
        
        char[] retorno_justo = new char[i];
        for(i=0;i<retorno_justo.length;i++){retorno_justo[i]=retorno[i];}
        
        return retorno_justo;
    }    
}
