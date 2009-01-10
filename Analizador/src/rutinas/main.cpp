#include <stdio.h>
#include "com/serial.h"

extern "C" char __stdcall lee(); // Linea extra�a...
extern "C" void __stdcall escribe(char); // Linea extra�a...

    DCB OldConf;
    HANDLE fd;
    #define LARGO_BUFFER 5000
    char buffer[LARGO_BUFFER];
    enum BUFF_ESTADO {LLENO=1, VACIO=0};
    BUFF_ESTADO b_estado;

int main(int argc, char* argv[])
{
    inicializar_serie();
    char letra;
    escribe('a');
    letra = lee();
    printf("Lee: %c.\n",letra);
    finalizar_serie(fd);
    return 0;
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
    OldConf=Get_Configure_Port(fd); // Guarda la configuracion del puerto.
    Configure_Port(fd,B115200,"8N1"); // Configura el puerto serie.

    return fd;
}

void finalizar_serie(HANDLE fd){
  Set_Configure_Port(fd,OldConf); // Restituye la antigua configuracion.
  Close_Port(fd);                 // Cierra el puerto serie.
}

