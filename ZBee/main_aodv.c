#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "zbee.h"

#define SOURCE "0008"
#define DESTINATION "0009"


void
print_RREQ(void* m)
{

	RREQ* msg;
	
	msg = (RREQ*)m;
	if (msg->message_id == MESSAGE_ID_RREQ)
	{
		printf(" RREQ(%d): \n\tsource=", (int)msg->message_id);
		print_address(msg->source);
		printf("\n\tdestination=");
		print_address(msg->destination);
		printf("\n\tlifespan=%03d\n\tid=%02d\n\n", msg->lifespan, msg->id);
	}
	else
	{
		printf("ERROR: RREQ expected.\n");
	}
}

void
print_RREP(void* m)
{

	RREP* msg;
	
	msg = (RREP*)m;
	if (msg->message_id == MESSAGE_ID_RREP)
	{
		printf(" RREP(%d): \n\tsource=", (int)msg->message_id);
		print_address(msg->source);
		printf("\n\tdestination=");
		print_address(msg->destination);
		printf("\n\tlifespan=%03d\n\tid=%02d\n\n", msg->lifespan, msg->id);
	}
	else
	{
		printf("ERROR: RREP expected.\n");
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



void
send_RREP(HANDLE fd, address source, address destination, address next_hop, int lifespan, int id)
{
	RREP rrep;
	address broadcast;
	
	init_address(broadcast, "FFFF");
	
	rrep.message_id = MESSAGE_ID_RREP;
	copy_addresses(rrep.source, source);
	copy_addresses(rrep.destination, destination);
	rrep.lifespan = lifespan;
	rrep.id = id;
	rrep.tail = 0;
	
	printf("Sending RREQ broadcast: ");
	print_RREP(&rrep);
	printf("\n");
	
	send_data_to(fd, broadcast, (void*)&rrep, sizeof(RREP));
}


void
read_one_message(HANDLE fd, char* buff)
{
    int size = read_all(fd, buff);
    if (size>0)
    {
        switch(buff[0])
        {
            case MESSAGE_ID_RREP:
		        print_RREP((char*)buff);
		        break;
            case MESSAGE_ID_RREQ:
		        print_RREQ((char*)buff);
		        break;
            default:
		        printf("Message not valid.\n");
	
        }
     }
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
                    read_one_message(fd, buff);
                    switch(buff[0])
                    {
                        case MESSAGE_ID_RREQ:
                            /* If the destination is in my link state table, reply RREP. */
							/* Else, forward RREQ. */
							
							/* Wrong supposition: I AM THE DESTINATION. RREP FORMAT IS OKAY (IT IS NOT!). */
							printf("Sending a WRONG RREP...\n");
							send_RREP(fd, source, destination, destination, 0, 0);
                            break;
                        default:
                            break;
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
                        

                        while(TRUE)
                        {
                            printf("Destination is NOT in the table. Sending RREQ...\n");
				            send_RREQ(fd, source, destination, destination, 0, 0); /* Send RREQ. */
                            /* Wait for the first RREP. The other RREP messages will 
                            be processed in other stage. */
                            printf("Waiting for the first RREP...\n"); 
                            Sleep(6000); 
                            
                            read_one_message(fd, buff);
                            if (buff[0] == MESSAGE_ID_RREP)
                            {
                                break;
                            }
                        }
                        printf("I have one route to the destination!\n");
						
						/*
						packet p = new packet(destination, data);
                        send_data_to(fd, destination_next_hop, p);
						*/
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
