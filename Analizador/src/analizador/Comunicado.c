#include <jni.h>
#include <stdio.h>
#include "analizador_Comunicador.h"
#define __LINUX_COM__ 
#include "com/serial.h"

#define LARGO_BUFFER 5000
enum BUFF_ESTADO {LLENO=1, VACIO=0};

DCB OldConf;
HANDLE fd;


enum BUFF_ESTADO b_estado;

char buffer[LARGO_BUFFER];



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
        Configure_Port(fd,B115200,"8N1"); // Configura el puerto serie.
        Set_Time(fd,50);                   // time-out entre caracteres es TIME*0.1
    }
    return fd;
}

void finalizar_serie(HANDLE fd){
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



JNIEXPORT jint JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    int retorno;
    fd = inicializar_serie("/dev/usb/ttyUSB0");
    retorno = (int)fd;
    return retorno;
}


JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    finalizar_serie(fd);
}




/*
 JNIEXPORT void JNICALL Java_Comunicador_enviarComando(JNIEnv *env, jobject obj, jstring comando){
		int suma;
    const char *str = env->GetStringUTFChars(comando, 0);
    printf("Estamos en C...\n");
 		suma = Addup (1, 1, 1);
    printf("Comando para el PIC: %s (suma %d)\n", str,suma);

}


JNIEXPORT jstring JNICALL Java_Comunicador_recibirComando(JNIEnv *env, jobject obj){
    const char retorno[] = "Enviado desde el PIC...";
    return env->NewStringUTF(retorno);
}


 */
