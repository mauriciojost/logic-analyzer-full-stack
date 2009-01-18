/* COMUNICADO DE LINUX */
#include <jni.h>
#include <stdio.h>
#include "analizador_Comunicador.h"
#define __LINUX_COM__ 
#include "com/serial.h"

#define LARGO_BUFFER 5000
enum BUFF_ESTADO {LLENO=1, VACIO=0};

void cerrar_serie();
HANDLE inicializar_serie(char* puerto);
int capturar_trama();
int es_analizador();

DCB OldConf;
HANDLE fd;


enum BUFF_ESTADO b_estado;

char buffer[LARGO_BUFFER];

JNIEXPORT jint JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    int retorno;
    int retorno;
    int i;
    char nombre_puerto[25];
    for(i=0;i<5;i++){
        sprintf(nombre_puerto, "COM%d",i+1);
        fd = inicializar_serie(nombre_puerto); 
        if (fd!=-1){
            if (es_analizador()){
                printf("Analizador encontrado! (%s).\n",nombre_puerto);
                break;
            }else{
                prinft("No se trata del analizador (%s).\n",nombre_puerto);
            }
        }else{
            printf("El puerto '%s' no es valido. \n",nombre_puerto);
        }
        
        sprintf(nombre_puerto, "/dev/usb/ttyUSB%1d",i);
        fd = inicializar_serie(nombre_puerto); 
        if (fd!=-1){
            if (es_analizador()){
                printf("Analizador encontrado! (%s).\n",nombre_puerto);
                break;
            }else{
                prinft("No se trata del analizador (%s).\n",nombre_puerto);
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
        //printf("\nEsto fue recibido en C luego de enviar '%s'\n",buffer);
    }else{
        b_estado=VACIO;
    }
}

JNIEXPORT jstring JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){
    jbyte retorno;
    char array[3];
    char caracter='9';
    unsigned int i = 0;
    int n = 0;


    if (b_estado==LLENO){
        //printf("El buffer estaba lleno al leer.\n");
        b_estado = VACIO;

    }else{
        //printf("El buffer estaba vacio al leer.\n");
        n=capturar_trama();
        if (n==0){b_estado=VACIO;buffer[0]=0;}else{b_estado=LLENO;}
    }
    //printf("Retorno para la lectura (C): '%s'.\n",buffer);

    return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    cerrar_serie();
}


HANDLE inicializar_serie(char* puerto){
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
        Set_Time(fd,50);                   // time-out entre caracteres es TIME*0.1
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

/*
 ********** NO ANDA!! JNIEXPORT void JNICALL Java_Comunicador_enviarComando(JNIEnv *env, jobject obj, jstring comando){
    int suma;
    const char *str = env->GetStringUTFChars(comando, 0);
    printf("Estamos en C...\n");
    suma = Addup (1, 1, 1);
    printf("Comando para el PIC: %s (suma %d)\n", str,suma);

}


********* NO ANDA!!! JNIEXPORT jstring JNICALL Java_Comunicador_recibirComando(JNIEnv *env, jobject obj){
    const char retorno[] = "Enviado desde el PIC...";
    return env->NewStringUTF(retorno);
}

 */
