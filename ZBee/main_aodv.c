#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "zbee.h"

#define SOURCE "0008"
#define DESTINATION "0009"
#define SERIAL_PORT "COM7"


void
print_RREQ(void* m)
{

	RREQ* msg;
	
	msg = (RREQ*)m;
	if (msg->message_id == MESSAGE_ID_RREQ)
	{
		printf(" RREQ(%d) source=", (int)msg->message_id);
		print_address(msg->source);
		printf(" destination=");
		print_address(msg->destination);
		printf(" lifespan=%03d id=%02d\n", msg->lifespan, msg->id);
	}
	else
	{
		printf("ERROR: RREQ expected.\n");
	}
}


void
send_RREQ(HANDLE fd, address source, address destination, address next_hop, int lifespan, int id)
{
	RREQ rreq;
	address broadcast;
	
	init_address(broadcast, "FFFF");
	
	rreq.message_id = MESSAGE_ID_RREQ;
	copy_addresses(rreq.source, source);
	copy_addresses(rreq.destination, destination);
	rreq.lifespan = lifespan;
	rreq.id = id;
	rreq.tail = 0;
	
	printf("Sending RREQ broadcast: ");
	print_RREQ(&rreq);
	printf("\n");
	
	send_data_to(fd, broadcast, (void*)&rreq, sizeof(RREQ));
}

int 
main()
{
    char buff[1024*8];
   
    routing_table_item table[ROUTING_TABLE_MAX_ITEMS];
    
    routing_table_item item;
    item.destination[0] = '7';

    initialize_routing_table(table);

    /* print_routing_table(table);
    item.destination[0] = '5';
    put_item_at_routing_table(table, item); */
    
   
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
    address destination;
    printf("Write local address (4 characters 0000-FFFF): ");
    gets(buff);
    init_address(source, buff);
    init_address(destination, DESTINATION);
	
	printf("Write serial port number (COM1-COM8): ");
    gets(buff);
    initialize_zigbee_module(&fd, buff, source);

    if (fd!=(HANDLE)-1)
    {
        printf("Connected to ZigBee module.\n");

        int exit = FALSE;

        while(exit==FALSE){
            int index;
            printf("\n\n*************\nInstructions: "
			" q (quit) p (proceed) r (read) c (change) s (send) h (show r. table)\n");
			
            char a = getch();           
            fflush(stdin);  

            switch(a)
            {
                case 'h': // Show routing table.
                    print_routing_table(table);
                    break; 

                case 'r': // Read. 
                    {
                    int size = read_all(fd, buff);
					if (size>0)
					{
						switch(buff[0])
						{
							case MESSAGE_ID_RREQ:
								print_RREQ((char*)buff);
								break;
							default:
								printf("Message not valid.\n");
							
						}
					
					}
                    }
                    break;

                case 'c': // Change data/destination.
                    {
                    printf("Please, write the new local address (4 characters, 0000-FFFF): ");
                    gets(buff);
                    init_address(source, buff);
                    change_local_address(fd, source);
                    printf("Please, write the new destination address (4 characters, 0000-FFFF): ");
                    gets(buff);
                    init_address(destination, buff);
                    printf("Please, write the new data (less than 20 characters): ");
                    gets(data);                    
                    }
                    break;
            
                case 's': // Send.

                    if ((index = get_index_item_routing_table(table, destination))!=-1)
                    {
                        /* Send Message (using the next hop). */
                        printf("Destination is in the table.\n");
                        /* send_to_through(fd, destination, next_hop, ttl). */
                        send_string_to(fd, destination, data);
                    }
                    else
                    {
                        
                        printf("Destination is NOT in the table.\n");
						send_RREQ(fd, source, destination, destination, 0, 0); /* Send RREQ. */
                        /*
						While(){
							Send RREQ.
							Wait for RREP. y salir
							
						}
                        */
                        //send_string_to(fd, destination, data);
                    }
                    break;

                case 'p': // Proceed.
					/* 
					Leer todo.
					Si es RREQ responder con un RREP si tengo en mi tabla.
					Si es mensaje con destino a mi, mostrar.
					Si es 
					*/
                    //read_all(fd, buff);
                    //send_to(fd, destination, "12345678");
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
