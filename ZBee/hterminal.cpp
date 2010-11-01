
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define __WINDOWS_COM__
#include "com/serial.h"

#define TRUE 1 
#define FALSE 0

DCB OldConf;
HANDLE fd;

int Read_Port_Blocking(HANDLE fd, char* buff);


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
    fd = Open_Port(puerto);             // Open the serial port. 
    OldConf = Get_Configure_Port(fd);   // Save the previous configuration. 
    Configure_Port(fd,B9600,"8N1");     // Use the current configuration. 
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


int 
check_OK_retrieved(HANDLE fd)
{
    char buff[1024];
    int size;
    int res = Read_Port_Blocking(fd, buff);
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
    int size_to_read; 
    int time_out=0;
    do
    {
        time_out++;
        Sleep(100);
        size_to_read = Kbhit_Port(fd); /* How many bytes are available to read? */
    }
    while(size_to_read == 0 && time_out < 30);

    Read_Port(fd, buff, size_to_read);
    printf("Read port blocking time_out %d size_to_read\n", time_out, size_to_read);

    return size_to_read;

}


int
write_AT_command(char* command)
{
    /* Clean the buffer. */
    int size;
    int res;
    char buff[1024];
    res = Clean_Buffer(fd);
    if (res != TRUE) force_exit("Clean buffer");

    Write_Port(fd,"+++",3);  /* Enter to command mode. */
    
    if (check_OK_retrieved(fd))
    {
        printf("Command mode.\n");            
        Write_Port(fd,command,strlen(command)); /* Send command. */
        if (check_OK_retrieved(fd))
        {
            printf("Command sent OK.\n");            
            return TRUE;       
        }
    }    
    
    printf("ERROR: Cannot send %s command.\n", command);
    return FALSE;
    
    
    
}

void
initialize_zigbee_module()
{
    int res; 
    char buff[1024];

    /* Initialize serial port. */
    fd = initialize_serial("COM7");

    write_AT_command("ATDL0000\r");

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
