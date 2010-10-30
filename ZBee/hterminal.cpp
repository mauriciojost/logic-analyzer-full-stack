#define __WINDOWS_COM__
#include "com/serial.h"
#include <stdio.h>
#include <conio.h>

DCB OldConf;

HANDLE inicializar_serie(char* puerto){
    HANDLE fd;
    fd=Open_Port(puerto);           // Abre el puerto serie.
    OldConf=Get_Configure_Port(fd); // Guarda la configuracion del puerto.
    Configure_Port(fd,B9600,"8N1"); // Configura el puerto serie.
    return fd;
}

void finalizar_serie(HANDLE fd){
  Set_Configure_Port(fd,OldConf); // Restituye la antigua configuracion.
  Close_Port(fd);                 // Cierra el puerto serie.
}

int main()
{
    HANDLE fd;

    char buff[5]="J";

    fd = inicializar_serie("COM6");


    while(TRUE){
        if(Kbhit_Port(fd)!=0){      // Existe algo para leer del puerto serie?
          Read_Port(fd,buff,1);			// En ese caso leerlo, y
          printf("%c",buff[0]);     // mostrarlo en consola.
        }
        if (kbhit()!=0){						// Se ha presionado una tecla?
          char a = getch();					// En ese caso, tomarla y
          buff[0] = a;							// enviarla por puerto serie.
          Write_Port(fd,buff,1);    // Escribe en el puerto serie.
        }
    }

    finalizar_serie(fd);

    return 0;
}

