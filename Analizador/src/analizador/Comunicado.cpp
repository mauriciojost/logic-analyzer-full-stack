#include <jni.h>
#include <stdio.h>
#include "analizador_Comunicador.h"

//extern "C" void __stdcall inicia(); // Linea extra�a...
//extern "C" char __stdcall lee(); // Linea extra�a...
//extern "C" void __stdcall escribe(char); // Linea extra�a...

const char* paquete = "<inicio nuevo=1 modo=1 velocidad=1000> 0 1 2 3 4 5 6 7 8 9 10 11 22 33 44 55 66 77 <CRC> 1123 </CRC> </inicio>\n";
int i=0;

JNIEXPORT void JNICALL Java_analizador_Comunicador_enviar(JNIEnv *env, jobject obj, jbyte c){
    printf("-%c",(jbyte)c);
    //escribe((jbyte)c);
}


JNIEXPORT jbyte JNICALL Java_analizador_Comunicador_recibir(JNIEnv *env, jobject obj){
    jbyte retorno;
    //retorno = (jbyte)lee();
    retorno = (jbyte)paquete[i++];
    //printf("Retorno en C:'%c'.\n",(jbyte)retorno);
    return retorno;
}



JNIEXPORT void JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    //inicia();
}


