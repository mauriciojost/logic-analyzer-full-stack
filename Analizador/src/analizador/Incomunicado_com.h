#define LARGO_BUFFER 5000
#define LARGO_COMANDO 100
char buffer[LARGO_BUFFER];
char comando[LARGO_COMANDO];

long int p_comando=0;
int nuevo, modo;
long velocidad;

void generar_trama(int nuevo, int modo, long velocidad){
    unsigned int j=0;
    char buffer_aux[60];

    sprintf(buffer_aux,"<i n=%u m=%u p=%lu> ",nuevo,modo,velocidad);
    strncpy(buffer, buffer_aux,60);
    //printf("Etapa 1: '%s'.\n",buffer);
    
    for(j=0;j<1024;j++){
        sprintf(buffer_aux,"%x ",j%256);
        strcat(buffer, buffer_aux);
        //printf("Etapa %d: '%s'.\n",2+j,buffer);
    }
    
    sprintf(buffer_aux , "<v> %d </v> </i>",0);
    strncat(buffer, buffer_aux,60);
    //printf("Etapa ultima: '%s'.\n",buffer);
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_enviar(JNIEnv *env, jobject obj, jbyte c){
    if (c==13){
        char buff_auxiliar[50];
        
        printf("Comando recibido (en C): '%s'.\n",comando);
        sscanf( comando, "\n<i n=%d m=%d p=%lu>%s", &nuevo,&modo,&velocidad,buff_auxiliar);
        printf("Interpretado: n=%d m=%d p=%lu.\nResto: '%s'.\n",nuevo,modo,velocidad,buff_auxiliar);
        
        p_comando = 0;
    }else{
        comando[p_comando++]=c;
        comando[p_comando]=0;
    }
}


JNIEXPORT jint JNICALL Java_analizador_Comunicador_iniciar(JNIEnv *env, jobject obj){
    unsigned int i;
    for(i=0;i<LARGO_BUFFER;i++)
        buffer[i]=0;   
    for(i=0;i<LARGO_COMANDO;i++)
        comando[i]=0;
    return 1;
}

JNIEXPORT void JNICALL Java_analizador_Comunicador_finalizar(JNIEnv *env, jobject obj){
    printf("Solicitud de cierre del puerto serie (en C).\n");
}

