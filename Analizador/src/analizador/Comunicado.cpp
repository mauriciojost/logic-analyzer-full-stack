/* COMUNICADO DE WINDOWS */
#include <jni.h>
#include <stdio.h>
#include "analizador_Comunicador.h"
#define __WINDOWS_COM__ 
#include "com/serial.h"


/******* FUNCIONES JNI COMPARTIDAS ********/
#include "Comunicado_com.h"
/******* FUNCIONES JNI  NO COMPARTIDAS ********/

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
    printf("Retorno para la lectura (C): '%s'.\n",buffer);

    return env->NewStringUTF(buffer);
}


