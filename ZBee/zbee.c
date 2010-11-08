#define __WINDOWS_COM__
#include "com/serial.h"

#include "zbee.h"

/*
// Simulation
#define TIMEOUT 1
#define SLEEP_UNIT 1
*/

// With hardware

#define TIMEOUT 100
#define SLEEP_UNIT 150


void 
finalize_serial(HANDLE fd)
{
    //Set_Configure_Port(fd,OldConf); // Restore previous configuration.
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
    //OldConf = Get_Configure_Port(fd);   // Save the previous configuration. 
    Configure_Port(fd,B9600,"8N1");     // Use the current configuration. 
    return fd;
}


int
read_all(HANDLE fd, char* data)
{
    
    int a;
    if((a = Kbhit_Port(fd))!=0){      // Is there something to read from serial port?
        Read_Port(fd,data,a);          // Then read it and show it.
        data[a] = 0;
        //printf("Data received: '%s'\n",data);
    }
    return a;
    
}

void
send_data_to(HANDLE fd, address dest, char* data, int size)
{
    char buff[16];

    printf("Sending data to %c%c%c%c: '%s'...\n", dest[0], dest[1], dest[2], dest[3], data);
    sprintf(buff, "ATDL%c%c%c%c", dest[0], dest[1], dest[2], dest[3]);
    write_AT_command(fd, (char*)buff); 
    Write_Port(fd,data,size);  // Write the serial port.    
}


void
send_string_to(HANDLE fd, address dest, char* data)
{
    send_data_to(fd, dest, data, strlen(data));
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
        Sleep(SLEEP_UNIT);
        size_to_read = Kbhit_Port(fd); /* How many bytes are available to read? */
    }
    while(size_to_read == 0 && time_out < TIMEOUT);

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


    Sleep(SLEEP_UNIT*10); /* No too many commands in a short period of time! */ 

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
change_local_address(HANDLE fd, address source)
{
    char command_my_address[10];
    sprintf(command_my_address, "ATMY%c%c%c%c", 
                                source[0], 
                                source[1],
                                source[2], 
                                source[3]);

    write_AT_command(fd, command_my_address);   /* MY ADDRESS */
}

void
initialize_zigbee_module(HANDLE* fdr, char* serial_port, address source)
{
    int res; 
    char buff[1024];

    HANDLE fd;
    /* Initialize serial port. */
    fd = initialize_serial(serial_port);

    printf("Initializing ZigBee module...\n");
    change_local_address(fd, source);
    write_AT_command(fd, "ATDH0000");           /* DEST. ADDRESS */
    write_AT_command(fd, "ATDLFFFF");           /* DEST. ADDRESS */
    write_AT_command(fd, "ATCH0x0C");           /* CHANNEL ID */
    write_AT_command(fd, "ATID0000");           /* PAN ID */
    write_AT_command(fd, "ATMM2");              /* ACK */

    *fdr = fd;
}


