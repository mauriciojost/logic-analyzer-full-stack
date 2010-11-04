#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "zbee.h"

#define SOURCE "0008"
#define DESTINATION "0009"
#define SERIAL_PORT "COM1"


void
print_instructions()
{
    printf("Instructions: \n  q (quit)\n  p (proceed)\n");
}

int 
main()
{

    char buff[1024*8];


    /*
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
    */
    
   


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

    HANDLE fd;
    char data[] = "*packet_content*                          ";
    
    address source;
    printf("Write local address (4 characters 0-F): ");
    gets(buff);
    init_address(source, buff);
    initialize_zigbee_module(&fd, SERIAL_PORT, source);

    

    if (fd!=(HANDLE)-1)
    {
        printf("Connected to ZigBee module.\n");

        int exit = FALSE;

        while(exit==FALSE){
            print_instructions();
            char a = getch();           
            fflush(stdin);  

            switch(a)
            {
                case 'r': // Read. 
                    read_all(fd, buff);
                    break;

                case 'c': // Change data/destination.
                    {
                    printf("Please, write the new address (4 characters, 0-F): ");
                    gets(buff);
                    init_address(source, buff);
                    printf("Please, write the new data (less than 20 characters): ");
                    gets(data);
                    
                    }
                    break;
            
                case 's': // Send.
                    send_to(fd, DESTINATION, data);
                    break;

            
                case 'p': // Proceed.
                    read_all(fd, buff);
                    send_to(fd, DESTINATION, "12345678");
                    break;

                case 'q': // Quit. 
                    exit = TRUE;
                    break;
                default: 
                    printf("Non valid option.\n");
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
