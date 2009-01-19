/* INCOMUNICADO DE WINDOWS */
#define __WINDOWS_COM__
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "analizador_Comunicador.h"
#include "Incomunicado_com.h"

JNIEXPORT jstring JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){

    generar_trama(nuevo,modo,velocidad);


    printf("Retorno para la lectura (desde C): '%s'.\n",buffer);

    return env->NewStringUTF(buffer);
}

