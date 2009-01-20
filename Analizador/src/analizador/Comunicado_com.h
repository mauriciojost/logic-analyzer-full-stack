/******* FUNCIONES JNI COMPARTIDAS ********/

HANDLE inicializar_serie(char* puerto);
int es_analizador();
void cerrar_serie();
int capturar_trama();

DCB OldConf;
HANDLE fd;

#define LARGO_BUFFER 5000
enum BUFF_ESTADO {LLENO=1, VACIO=0};

char buffer[LARGO_BUFFER];
enum BUFF_ESTADO b_estado;


JNIEXPORT jint JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    int retorno;
    int i;
    char nombre_puerto[25];
    for(i=0;i<5;i++){
        sprintf(nombre_puerto, "COM%d",i+1);
        fd = inicializar_serie(nombre_puerto);
        if (((int)fd)!=-1){
            if (es_analizador()){
                printf("Analizador encontrado! (%s).\n",nombre_puerto);
                Set_Time(fd,50);                   // time-out entre caracteres es TIME*0.1
                break;
            }else{
                printf("No se trata del analizador (%s).\n",nombre_puerto);
            }
        }else{
            printf("El puerto '%s' no es valido. \n",nombre_puerto);
        }

        sprintf(nombre_puerto, "/dev/usb/ttyUSB%1d",i);
        fd = inicializar_serie(nombre_puerto);
        if (((int)fd)!=-1){
            if (es_analizador()){
                printf("Analizador encontrado! (%s).\n",nombre_puerto);
                Set_Time(fd,50);                   // time-out entre caracteres es TIME*0.1
                break;
            }else{
                printf("No se trata del analizador (%s).\n",nombre_puerto);
            }
        }else{
            printf("El puerto '%s' no es valido. \n",nombre_puerto);
        }

        sprintf(nombre_puerto, "/dev/ttyS%1d",i);
        fd = inicializar_serie(nombre_puerto);
        if (((int)fd)!=-1){
            if (es_analizador()){
                printf("Analizador encontrado! (%s).\n",nombre_puerto);
                Set_Time(fd,50);                   // time-out entre caracteres es TIME*0.1
                break;
            }else{
                printf("No se trata del analizador (%s).\n",nombre_puerto);
            }
        }else{
            printf("El puerto '%s' no es valido. \n",nombre_puerto);
        }

    }
    retorno = (int)fd;
    return retorno;
}




JNIEXPORT void JNICALL Java_analizador_Comunicador_enviar(JNIEnv *env, jobject obj, jbyte c){
    char array[3];
    char caracter='9';
    unsigned int i=0;
    int n=0;

    array[0] = c;
    Write_Port(fd,array,1);    // Escribe en el puerto serie.

    if (Kbhit_Port(fd)!=0){
        //printf("Hay algo...\n");
        n=capturar_trama();
        if (n==0){b_estado=VACIO;buffer[0]=0;}else{b_estado=LLENO;}
        printf("\nEsto fue recibido en C luego de enviar '%s'\n",buffer);
    }else{
        b_estado=VACIO;
    }
}


JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    cerrar_serie();
}


HANDLE inicializar_serie(char* puerto){
    HANDLE fd;
    char array[3];
    int i;

    for(i=0;i<LARGO_BUFFER;i++){
        buffer[i]=0;
    }
    b_estado = LLENO;
    fd=Open_Port(puerto);           // Abre el puerto serie.

    if ((int)fd != -1){
        OldConf=Get_Configure_Port(fd);   // Guarda la configuracion del puerto.
        //Configure_Port(fd,B115200,"8N1"); // Configura el puerto serie.
        Configure_Port(fd,B460800,"8N1"); // Configura el puerto serie.
        Set_Time(fd,5);                   // time-out entre caracteres es TIME*0.1
    }
    return fd;
}

void cerrar_serie(){
  Set_Configure_Port(fd,OldConf); // Restituye la antigua configuracion.
  Close_Port(fd);                 // Cierra el puerto serie.
}

int capturar_trama(){
    char array[3];
    char caracter;
    unsigned int k = 0;
    unsigned int i = 0;
    int n=0;

    n=1;
    while(caracter!='\n'){
        k = Getc_Port(fd,&caracter);
        if (k==0){
            n=0;
            break;
        }
        //Gets_Port(fd,array,1);
        //Read_Port(fd,array,1);			// ARREGLAR!!! PUEDE QUE DEVUELVA BASURA!!!
        //caracter = (jbyte)array[0];
        buffer[i++] = caracter;
    }
    buffer[i-1]=0; // Finaliza el string.
    return n;
}


int es_analizador(){
    char array[3];
    char caracter;
    char caracter_id_solic='*';
    char caracter_id_resp='#';
    
    unsigned int n_carac_leidos = 0;
    
    
    array[0]=caracter_id_solic;
    Write_Port(fd,array,1);             // Escribe en el puerto serie el caracter de solicitud de ID.
    n_carac_leidos = Getc_Port(fd,&caracter);        // Recibe y compara.
    printf("Verificando ID. Enviado '%c' y recibido '%c' (%d caracteres, '%c' esperado).\n",caracter_id_solic, caracter, n_carac_leidos, caracter_id_resp);
    if ((n_carac_leidos==1) && (caracter==caracter_id_resp)){
        return 1;
    }else{
        return 0;
    }
}




