#define __WINDOWS_COM__
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "analizador_Comunicador.h"

#define LARGO_BUFFER 5000
#define LARGO_COMANDO 100
char buffer[LARGO_BUFFER];
char comando[LARGO_COMANDO];

long int p_comando=0;
enum BUFF_ESTADO {LLENO=1, VACIO=0};
BUFF_ESTADO b_estado;


void generar_trama(int nuevo, int modo, long velocidad){
    unsigned int j=0;
    char buffer_aux[60];

    sprintf(buffer_aux,"<inicio nuevo=%u modo=%u velocidad=%lu> ",nuevo,modo,velocidad);
    strncpy(buffer, buffer_aux,60);

    //printf("Etapa 1: '%s'.\n",buffer);
    
    for(j=0;j<1024;j++){
        sprintf(buffer_aux,"%d ",j%256);
        strcat(buffer, buffer_aux);
        //printf("Etapa %d: '%s'.\n",2+j,buffer);
    }
    
    sprintf(buffer_aux , "<CRC> %d </CRC> </inicio>",123);
    strncat(buffer, buffer_aux,60);
    //printf("Etapa ultima: '%s'.\n",buffer);
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_enviar(JNIEnv *env, jobject obj, jbyte c){
    if (c==13){
        printf("Comando recibido (C): %s\n.",comando);
        p_comando = 0;
    }else{
        comando[p_comando++]=c;
        comando[p_comando]=0;
    }
}

JNIEXPORT jstring JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){
    int nuevo, modo;
    long velocidad;
    
    generar_trama(1,0,1000);
    
    sscanf( buffer, "<inicio nuevo=%d modo=%d velocidad=%lu>", &nuevo,&modo,&velocidad);
    
    printf("Retorno para la lectura (C): '%s'.\n",buffer);
    printf("Interpretado: nuevo=%d modo=%d velocidad=%lu.\n",nuevo,modo,velocidad);
    return env->NewStringUTF(buffer);
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    unsigned int i;
    for(i=0;i<LARGO_BUFFER;i++)
        buffer[i]=0;   
    for(i=0;i<LARGO_COMANDO;i++)
        comando[i]=0;
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    
}

