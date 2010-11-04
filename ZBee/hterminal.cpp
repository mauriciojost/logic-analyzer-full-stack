#include <stdio.h>
#include <conio.h>
#include <string.h>

#define __WINDOWS_COM__
#include "com/serial.h"

#include "misc.h"


#define SOURCE "0008"
#define DESTINATION "0009"
#define SERIAL_PORT "COM7"


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
send_to(HANDLE fd, address dest, char* data)
{
    int size; 
    char buff[16];

    printf("Sending data to %c%c%c%c: %s\n", dest[0], dest[1], dest[2], dest[3], data);
    sprintf(buff, "ATDL%c%c%c%c", dest[0], dest[1], dest[2], dest[3]);
    write_AT_command(fd, (char*)buff); 
    Write_Port(fd,data,strlen(data));  // Write the serial port.    
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
    fd = initialize_serial(SERIAL_PORT);

    
    /*
    CH CHANNEL          C
    ID PHAN ID          0000
    DL DEST. ADDRESS    XXXX
    DH DEST. ADDRESS    0000
    MY MY ADDRESS       XXXX

    MM CONF.            2
    */
    
    write_AT_command(fd, "ATCH0x0C");       /* CHANNEL ID */
    write_AT_command(fd, "ATID0000");       /* PAN ID */
    write_AT_command(fd, "ATDH0000");       /* DEST. ADDRESS */
    write_AT_command(fd, "ATDLFFFF");       /* DEST. ADDRESS */
    write_AT_command(fd, "ATMY" SOURCE);    /* MY ADDRESS */
    write_AT_command(fd, "ATMM2");          /* ACK */

   

    system("PAUSE");

    *fdr = fd;
    /* Initialization stuff. */
}






/****************************/
/* Things in the main file. */
/****************************/


int 
main()
{

    char buff[1024*8];

    HANDLE fd;
    //initialize_zigbee_module(&fd);


    routing_table_item table[ROUTING_TABLE_MAX_ITEMS];
    routing_table_item item;
    item.destination[0] = '7';

    initialize_routing_table(table);
    print_routing_table(table);
    system("pause");

    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");
    
    item.destination[0] = '5';
    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");

    item.destination[0] = '3';
    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");

    item.destination[0] = '1';
    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");

    item.destination[0] = '5';
    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");

    remove_item_routing_table(table, item.destination);
    print_routing_table(table);
    system("pause");

    item.destination[0] = '9';
    put_item_at_routing_table(table, item);
    print_routing_table(table);
    system("pause");


    
    exit(1);
    initialize_routing_table(table);
    print_routing_table(table);
    system("pause");

    initialize_routing_table(table);
    print_routing_table(table);
    system("pause");
   


    /*
    
    Requestor
    I want to send a message to node X.
    Check whether we have him in our routing table or not.
      If he's not in our routing table, just send RREQ.
    If I don't receive a route in certain amount of time, I will 
    resend the message (longer lifespane and other ID). 

    Requested
    I receive a RREQ. 
        Do I know a route to the destination?
        Yes
            Send a RREP. 
        No
            Re-broadcast RREQ (until lifespan is dead). Be sure not to
            rebroadcast a message with the same sequence number. 
            
    If I receive a RREP... I have to update routes that go the destination of
    the route reply.     

    Each node has a SN that will be updated each time it sends a message. It 
    represents how fresh is the information about itself. 

    Case for 3 nodes maximun.
        
    */    

    /*
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
    */
    return 0;
}
