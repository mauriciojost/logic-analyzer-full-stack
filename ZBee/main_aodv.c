#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#include "zbee.h"

#define SOURCE "0008"
#define DESTINATION "0009"


void
print_general_message(void* m)
{
	general_message* msg;
	
	msg = (general_message*)m;
	if (msg->message_id == MESSAGE_ID_RREQ)
	{
		printf(" Message ID=%d", (int)msg->message_id);
		printf("\n\tsource=");
		print_address(msg->source);
		printf("\n\tdestination=");
		print_address(msg->destination);
		printf("\n\taodv_source=");
		print_address(msg->source);
		printf("\n\taodv_destination=");
		print_address(msg->destination);
		printf("\n\tlifespan=%03d\n\tsequence_number_message(id)=%02d\n\n", msg->lifespan, msg->sequence_number_message);
	}
	else
	{
		printf("ERROR: Cannot print this message.\n");
	}
}

void
send_general_message(HANDLE fd, 
					int message_id, 
					address source, 
					address destination, 
					int hop_count,
					address aodv_source, 
					address aodv_destination,
					int lifespan, 
					int sequence_number_message,
					char* data)
{
	general_message gm;
	
	gm.message_id = message_id;
	copy_addresses(gm.source, source);
	copy_addresses(gm.destination, destination);
	gm.hop_count = hop_count;
	copy_addresses(gm.aodv_source, aodv_source);
	copy_addresses(gm.aodv_destination, aodv_destination);
	gm.lifespan = lifespan;
	gm.sequence_number_message = sequence_number_message;
	
	if (data!=NULL && strlen(data)>=DATA_FIELD_LENGTH)
	{
		strncpy(gm.data, data, DATA_FIELD_LENGTH);
	}	
	else
	{
		printf("Message data too short. Filling with 'AA...'.");
		strncpy(gm.data, "AAAAAAAA...", DATA_FIELD_LENGTH);
	}
	
	gm.tail = 0;
	
	printf("Sending message: ");
	print_general_message(&gm);
	printf("\n");
	
	send_data_to(fd, destination, (void*)&gm, sizeof(general_message));
}


void
read_one_message(HANDLE fd, char* buff)
{
	int size;
	
	buff[0] = MESSAGE_ID_INVALID;
    size = read_all(fd, buff);
	
    if (size>0)
    {
        print_general_message((char*)buff);
        switch(buff[0])
        {
            case MESSAGE_ID_RREP:        
		        break;
            case MESSAGE_ID_RREQ:
		        break;
            /*case MESSAGE_ID_HELL:
                break; */
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
    
	address broadcast; 
	init_address(broadcast, "FFFF");
    address source;
    address destination;
    printf("Write local address (4 characters 0000-FFFF, for example '0001'): ");
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
							printf("Message RREQ received: \n");
							
							general_message* gm = (general_message*)buff; 
                            /* If the destination is in my link state table, reply RREP. */
							// if ((index = get_index_item_routing_table(table, gm.destination)!=-1) && (table[index].hop_count==1))
								print_general_message(buff);
							
								//send_general_message(fd, MESSAGE_ID_RREP, source, table[index].next_hop, 
								//	0, source,  destination, DEFAULT_TTL, message_sequence_number, NULL);
							
								send_general_message(fd, MESSAGE_ID_RREP, source, destination, /* WRONG! NOT TO DESTINATION! */
									0, source,  destination, DEFAULT_TTL, gm->sequence_number_message, NULL);
							/* Else, forward RREQ. */
							/*
							printf("Forwarding RREQ...\n");
							
							send_general_message(fd, MESSAGE_ID_RREQ, source, broadcast, 
												gm., source,  destination, DEFAULT_TTL, message_sequence_number, NULL);
								
							
							*/
							
												
					
                            break;
							
						case MESSAGE_ID_RREP:
							/* Update your table!!! */
							break;
                        default:
                            break;
                    }
                    break;

                case 'c': // Change data/destination.
                    {
                    //printf("Please, write the new local address (4 characters, 0000-FFFF): ");
                    //gets(buff);
                    //init_address(source, buff);
                    //change_local_address(fd, source);
                    printf("Please, write the new destination address (4 characters, 0000-FFFF): ");
                    gets(buff);
                    init_address(destination, buff);
                    printf("Please, write the new data (less than 20 characters): ");
                    gets(data);                    
                    }
                    break;
            
                case 's': // Send.
					printf("Sending a message...\nPlease, write the new destination address (4 characters, 0000-FFFF): ");
                    gets(buff);
                    init_address(destination, buff);
                    printf("Please, write the new data (less than 20 characters): ");
                    gets(data);                    
					
					int route_found = FALSE;
					
                    if ((index = get_index_item_routing_table(table, destination))!=-1)
                    {
                        /* Send Message (using the next hop). */
                        printf("Destination is already in the routing table. Checking validity...\n");
						
						time_t current_time = time (NULL);
						printf("Checking time of the route: current=[%ld] initial_time+timeout=[%ld]\n");
						if (table[index].initial_time + ROUTE_TIMEOUT > current_time)
						{
							/* It's okay. We can use this route now. */
							
							route_found = TRUE;
							printf("Route found!\n");
							
						}
                    }
					
                    if (route_found == FALSE)
                    {
                        while(TRUE)
                        {
                            printf("Destination is NOT in the table. Sending RREQ...\n");
				            int message_sequence_number = rand();
							send_general_message(fd, MESSAGE_ID_RREQ, source, broadcast, 
												0, source,  /* Looking for ... */ destination , 
												DEFAULT_TTL, message_sequence_number, NULL);
                            /* Wait for the first RREP. The other RREP messages will 
                            be processed in other stage. */
                            printf("Waiting for the first RREP...\n"); 
                            Sleep(7000); 
                            
                            read_one_message(fd, buff);
                            if (buff[0] == MESSAGE_ID_RREP) /* The expected message was received. */
                            {
                                break;
                            }
                        }
						
                        
						printf("Now sending message...\n");
						int message_sequence_number = rand();
						send_general_message(fd, MESSAGE_ID_DATA, source, table[index].next_hop, 
								0, source,  destination, DEFAULT_TTL, message_sequence_number, NULL);
						
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
