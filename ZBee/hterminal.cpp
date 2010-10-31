#define __WINDOWS_COM__
#include "com/serial.h"
#include <stdio.h>
#include <conio.h>

DCB OldConf;
 
HANDLE initialize_serial(char* puerto){
    HANDLE fd;

    fd=Open_Port(puerto);           // Open the serial port. 
    OldConf=Get_Configure_Port(fd); // Save the previous configuration. 
    Configure_Port(fd,B9600,"8N1"); // Use the current configuration. 
    return fd;
}

void finalize_serial(HANDLE fd){
  Set_Configure_Port(fd,OldConf); // Restore previous configuration.
  Close_Port(fd);                 // Close.
}

int main()
{
    HANDLE fd;

    char buff[5]="J";

    fd = initialize_serial("COM6");

    while(TRUE){
        if(Kbhit_Port(fd)!=0){      // Is there something to read from serial port?
          Read_Port(fd,buff,1);		// Then read it and show it.
          printf("%c",buff[0]);
        }
        if (kbhit()!=0){			// Pressed key?
          char a = getch();			// Catch it, and send it through the serial port.
          buff[0] = a;							
          Write_Port(fd,buff,1);    // Write the serial port. 
        }
    }

    finalize_serial(fd);

    return 0;
}

