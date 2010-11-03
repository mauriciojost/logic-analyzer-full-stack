#include <stdio.h>
#include <conio.h>
#include <string.h>

#define __WINDOWS_COM__
#include "com/serial.h"

#define TRUE 1 
#define FALSE 0

#define SOURCE "0008"
#define DESTINATION "0009"

DCB OldConf;



int Read_Port_Blocking(HANDLE fd, char* buff);
int write_AT_command(HANDLE fd, char* command);

void 
finalize_serial(HANDLE fd)
{
    Set_Configure_Port(fd,OldConf); // Restore previous configuration.
    Close_Port(fd);                 // Close.
}


int 
force_exit(HANDLE fd, char* string)
{
    printf("FORCE EXIT: %s\n", string); 
    finalize_serial(fd);
    system("PAUSE");
    exit(-1);
}

 
HANDLE 
initialize_serial(char* puerto)
{
    HANDLE fd; 
    fd = Open_Port(puerto);             // Open the serial port. 
    OldConf = Get_Configure_Port(fd);   // Save the previous configuration. 
    Configure_Port(fd,B9600,"8N1");     // Use the current configuration. 
    return fd;
}


void
read_all(HANDLE fd, char* data)
{
    
    int a;
    if((a = Kbhit_Port(fd))!=0){      // Is there something to read from serial port?
        Read_Port(fd,data,a);	      // Then read it and show it.
        data[a] = 0;
        printf("Data received: %s\n",data);
    }
    
}


void
send_to(HANDLE fd, char* destination, char* data)
{
    int size; 
    char buff[16];
    if (strlen(destination)<=4)
    {
        printf("Sending data to %s: %s\n", destination, data);
        sprintf(buff, "ATDL%s", destination);
        write_AT_command(fd, (char*)buff); 
        Write_Port(fd,data,strlen(data));  // Write the serial port. 
    }
    else
    {
        printf("Error trying to interpret destination... More than 4 characters.\n");
    }
    
}


int 
check_OK_retrieved(HANDLE fd)
{
    char buff[1024];
    int size;
    int res;

    //printf("Waiting for OK response..."); 
    res = Read_Port_Blocking(fd, buff);
    if (res > 1)
    {
        if (strncmp("OK", buff, 2)==0)
            return TRUE;
    }
    return FALSE;
}

int
Read_Port_Blocking(HANDLE fd, char* buff)
{    
    int size_to_read = 0; 
    int time_out=0;
    do
    {
        time_out++;
        Sleep(150);
        size_to_read = Kbhit_Port(fd); /* How many bytes are available to read? */
    }
    while(size_to_read == 0 && time_out < 100);

    Read_Port(fd, buff, size_to_read);
    //printf("Read port blocking time_out %d size_to_read %d\n", time_out, size_to_read);

    return size_to_read;
}


int
write_AT_command(HANDLE fd, char* command)
{
    /* Clean the buffer. */
    int size;
    int res;
    char buff[1024];


    Sleep(1500); /* No too many commands in a short period of time! */ 

    res = Clean_Buffer(fd);
    if (res != TRUE) 
        force_exit(fd, "Clean buffer");

    printf("AT Command: %s\n", command);
    Write_Port(fd,"+++",3);  /* Enter to command mode. */
    
    if (check_OK_retrieved(fd))
    {
        Write_Port(fd,command,strlen(command)); /* Send command. */
        Write_Port(fd,"\r",1);                  /* Enter command. */
        if (check_OK_retrieved(fd))
        {

            Write_Port(fd,"ATCN",4);          /* Exit command mode. */
            //Sleep(500);
            Write_Port(fd,"\r" , 1);
            if (check_OK_retrieved(fd))
            {
                printf("Command sent OK.\n");            

                return TRUE;       
            }
            
        }
    }    
    printf("ERROR: Cannot send %s command.\n", command);
    return FALSE;
    
    
    
}

void
initialize_zigbee_module(HANDLE* fdr)
{
    int res; 
    char buff[1024];

    HANDLE fd;
    /* Initialize serial port. */
    fd = initialize_serial("COM7");

    
    /*
    CH CHANNEL          C
    ID PHAN ID          0000
    DL DEST. ADDRESS    XXXX
    DH DEST. ADDRESS    0000
    MY MY ADDRESS       XXXX

    MM CONF.            2
    */


    
    write_AT_command(fd, "ATID0000"); 
    write_AT_command(fd, "ATDH0000"); 
    write_AT_command(fd, "ATDLFFFF"); 
    write_AT_command(fd, "ATMY" SOURCE); 
    write_AT_command(fd, "ATMM2");

   

    system("PAUSE");

    *fdr = fd;
    /* Initialization stuff. */
}

int 
main()
{

    char buff[1024*8];

    HANDLE fd;
    initialize_zigbee_module(&fd);

    if (fd!=(HANDLE)-1)
    {
        printf("Connected to ZigBee module.\n");
        while(TRUE){
              
            send_to(fd, DESTINATION, "12345678");
            printf("Press q to finish...\n");
            char a = getch();       
            if (a=='q')
            {
                break;
            }
            read_all(fd, buff);
            
        }
    
        finalize_serial(fd);
    }
    else
    {
        printf("ERROR: Cannot connect to ZigBee module.\n");
    }
    return 0;
}
