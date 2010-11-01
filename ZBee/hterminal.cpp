
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define __WINDOWS_COM__
#include "com/serial.h"

DCB OldConf;
HANDLE fd;


void 
finalize_serial(HANDLE fd)
{
    Set_Configure_Port(fd,OldConf); // Restore previous configuration.
    Close_Port(fd);                 // Close.
}


int 
force_exit(char* string)
{
    printf("FORCE EXIT: %s\n", string); 
    finalize_serial(fd);
    system("PAUSE");
    exit(-1);
}
 
HANDLE 
initialize_serial(char* puerto)
{
    fd=Open_Port(puerto);           // Open the serial port. 
    OldConf=Get_Configure_Port(fd); // Save the previous configuration. 
    Configure_Port(fd,B9600,"8N1"); // Use the current configuration. 
    return fd;
}


void
read_all(char* data)
{
    
    if(Kbhit_Port(fd)!=0){      // Is there something to read from serial port?
        Read_Port(fd,data,1);	// Then read it and show it.
        printf("%c",data[0]);
    }
    
}

void
send_to(char* destination, char* data, int size)
{
    /*
    if (kbhit()!=0){			// Pressed key?
        char a = getch();       // Catch it, and send it through the serial port.
        buff[0] = a;							
        Write_Port(fd,buff,1);  // Write the serial port. 
    }
    */

    Write_Port(fd,data,size);  // Write the serial port. 
    
}

void Read_Port_Blocking(HANDLE fd, char* buff, int* size)
{    
    int size_to_read; 
    do
    {
        Sleep(100);
        size_to_read = Kbhit_Port(fd); /* How many bytes are available to read? */
    }
    while(size_to_read == 0);

    Read_Port(fd, buff, size_to_read);
    *size = size_to_read;
}

void 
write_AT_command(char* command)
{
    /* Clean the buffer. */
    int size;
    int res;
    char buff[1024];
    res = Clean_Buffer(fd);
    if (res != TRUE) force_exit("Clean buffer");

    long nr = Write_Port(fd,"+++",3);  /* Enter to command mode. */
    if (nr<0) force_exit("Write_Port to enter to command mode");
    
    Read_Port_Blocking(fd, buff, &size);
    buff[size] = 0;
    printf("Received command %s\n", buff);
    system("PAUSE");
}

void
initialize_zigbee_module()
{
    int res; 
    char buff[1024];

    /* Initialize serial port. */
    fd = initialize_serial("COM7");

    write_AT_command(NULL);
    system("PAUSE");

    /* Initialization stuff. */
}

int 
main()
{

    char buff[100];

    initialize_zigbee_module();

    if (fd!=(HANDLE)-1)
    {
        printf("Connected to ZigBee module.\n");
        while(TRUE){
            
            
            send_to("1111", "1234", 4);
            read_all(buff);
            printf("Press f to finish...\n");
            char a = getch();       
            if (a=='f')
            {
                break;
            }
            
        }
    
        finalize_serial(fd);
    }
    else
    {
        printf("ERROR: Cannot connect to ZigBee module.\n");
    }
    return 0;
}
