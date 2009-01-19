/* INCOMUNICADO DE LINUX */
#define __WINDOWS_COM__
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "analizador_Comunicador.h"
#include "Incomunicado_com.h"

JNIEXPORT jstring JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){
    int entero, a=10, b=20;
    generar_trama(nuevo,modo,velocidad);
    entero = _funcion(a,b);
    
    printf("Retorno para la lectura (desde C (suma en ASM: %d )): '%s'.\n",entero,buffer);
    
    return (*env)->NewStringUTF(env,buffer);
}


