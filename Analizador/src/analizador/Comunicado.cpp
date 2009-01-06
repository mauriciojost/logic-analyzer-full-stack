#define __WINDOWS_COM__
#include <jni.h>
#include <stdio.h>
#include "analizador_Comunicador.h"
#include "com/serial.h"


DCB OldConf;
HANDLE fd;
#define LARGO_BUFFER 5000
char buffer[LARGO_BUFFER];

//extern "C" void __stdcall inicia(); // Linea extra�a...
//extern "C" char __stdcall lee(); // Linea extra�a...
//extern "C" void __stdcall escribe(char); // Linea extra�a...


HANDLE inicializar_serie(char* puerto){
    HANDLE fd;
    char array[3];
    int i;
    
    for(i=0;i<LARGO_BUFFER;i++){
        buffer[i]=0;
    }
    
    fd=Open_Port(puerto);           // Abre el puerto serie.
    OldConf=Get_Configure_Port(fd); // Guarda la configuracion del puerto.
    Configure_Port(fd,B9600,"8N1"); // Configura el puerto serie.
    
    return fd;
}

void finalizar_serie(HANDLE fd){
  Set_Configure_Port(fd,OldConf); // Restituye la antigua configuracion.
  Close_Port(fd);                 // Cierra el puerto serie.
}


JNIEXPORT void JNICALL Java_analizador_Comunicador_enviar(JNIEnv *env, jobject obj, jbyte c){
    char array[3];
    
    // borrar de aca
    
    char caracter='9';
    unsigned int i=0;
    // hasta aca
    
    printf("-%c",(jbyte)c);
    array[0] = c;							// enviarla por puerto serie.
    Write_Port(fd,array,1);    // Escribe en el puerto serie.
    
    // y de aca
    for(i=0;i<100000;i++){
        array[0]=c;
    }
    
        i=0;
    if (Kbhit_Port(fd)!=0){    // BLOQUEANTE O NO????
        printf("Algo para revisar después de enviar...\n",buffer);
        while(caracter!='\n'){
            Read_Port(fd,array,1);			// ARREGLAR!!! PUEDE QUE DEVUELVA BASURA!!!
            caracter = (jbyte)array[0];
            buffer[i++] = caracter;
        }
        printf("\nEsto fue recibido en C luego de enviar '%s'\n",buffer);
    }
    // hasta aca
        

}


JNIEXPORT jbyte JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){
    jbyte retorno;
    char array[5000];
    char caracter='9';
    unsigned int i = 0;
    
    //while(caracter!='\n'){
	//if Kbhit_Port(fd)!=0{    // BLOQUEANTE O NO????
    	Read_Port(fd,array,1);			// ARREGLAR!!! PUEDE QUE DEVUELVA BASURA!!!
    	caracter = (jbyte)array[0];
        array[i++] = caracter;
  	//}
        printf("'%c'",caracter);
    //}
    //printf("Recibido en C: '%s'.\n",array);
    return array[0];
}



JNIEXPORT void JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    fd = inicializar_serie("COM1");
}


JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    finalizar_serie(fd);
}




