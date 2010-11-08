#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "zbee.h"


void
print_general_message(void* m)
{
    general_message* msg;
    
    msg = (general_message*)m;
    switch(msg->message_id)
    {
        case MESSAGE_ID_RREQ:
            printf("\nRREQ message"); 
            break;
        case MESSAGE_ID_RREP: 
            printf("\nRREP message"); 
            printf("\n  Sequence Number of Destination = %d", msg->sequence_number_destination);
            break;
        case MESSAGE_ID_HELL:
            printf("\nHELLO message"); 
            printf("\n  Sequence Number of Source = %d", msg->sequence_number_destination);
            break;
        case MESSAGE_ID_DATA:
            printf("\nDATA message"); 
            printf("\n  Data = '%s'", msg->data);
            break;
        default: 
            printf("\nUNKNOWN MESSAGE");
    }
    printf("\n  Source = "); print_address(msg->source);
    printf("\n  Destination = "); print_address(msg->destination);
    printf("\n  AODV Source = "); print_address(msg->aodv_source);
    printf("\n  AODV Destination = "); print_address(msg->aodv_destination);
    printf("\n  Hop count = %d" , msg->hop_count);
    printf("\n  Sequence Number Message (ID) = %d", msg->sequence_number_message);
    printf("\n  Lifespan = %d\n\n", msg->lifespan);
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
                    int sequence_number_destination, 
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
    gm.sequence_number_destination = sequence_number_destination;
    if (data!=NULL && strlen(data)>=DATA_FIELD_LENGTH)
    {
        strncpy(gm.data, data, DATA_FIELD_LENGTH);
    }    
    else
    {
        strncpy(gm.data, "-----------------------", DATA_FIELD_LENGTH);
    }
    
    gm.tail = 0;
    
    printf("\nSending the following message: ");
    print_general_message(&gm);
    printf("\n");
    
    send_data_to(fd, destination, (void*)&gm, sizeof(general_message));
}


void
send_hello_message(HANDLE fd, address my_address, int* my_sequence_number)
{
    address broadcast; 
    init_address(broadcast, "FFFF");

    *my_sequence_number = (*my_sequence_number) + 1;
    send_general_message(fd, MESSAGE_ID_HELL, my_address, broadcast, INITIAL_HOP_COUNT, 
        my_address, broadcast, DEFAULT_TTL, rand(), *my_sequence_number, NULL);
}

void
read_one_message(HANDLE fd, char* buff)
{
    int size;
    
    buff[0] = MESSAGE_ID_INVALID;
    size = read_all(fd, buff);
    
    if (size>0)
    {
        switch(buff[0])
        {
            case MESSAGE_ID_RREP:        
                printf("******** RREP message arrived.\n");
                break;
            case MESSAGE_ID_RREQ:
                printf("******** RREQ message arrived.\n");
                break;
            case MESSAGE_ID_HELL:
                printf("******** HELLO message arrived.\n");
                break;
            case MESSAGE_ID_DATA:
                printf("******** DATA message arrived.\n");
                break;
            default:
                printf("******** UNKNOWN message arrived.\n");    
                print_general_message((void*)buff); 
        }
    }
    else
    {
        printf("No message received.\n");
    }
     
}

void
use_HELLO_to_update_routing_table(routing_table_item table[], general_message* gm)
{

    printf("Updating routing table with the following HELLO message: \n");
    print_general_message(gm); 

    routing_table_item item;
    /* 
    We receive a message that must be used to fill the table in this way:
        TABLE                Msg. RREP            
        destination <-       source_aodv
        next_hop <-          source (physical source)
        number_of_hops <-    1
        initial_time <-      time(NULL)
        sequence_numb... <-  sequence_number_destination
    */

    copy_addresses(item.aodv_destination, gm->aodv_source);
    copy_addresses(item.next_hop, gm->source);
    item.number_of_hops = 1;
    item.initial_time = time(NULL);
    item.sequence_number_destination = gm->sequence_number_destination;
  
    put_item_at_routing_table(table, item);
    printf("New neighbor's route (one hop) added to the routing table.\n");

}

void
use_RREP_to_update_routing_table(routing_table_item table[], general_message* gm)
{
    printf("Updating routing table with RREP... Using the following message: \n");
    print_general_message(gm); 

    routing_table_item item;
    /* 
    We receive a message that must be used to fill the table in this way:
        TABLE                Msg. RREP            
        destination    <-    destination_aodv
        next_hop <-          source (physical source)
        number_of_hops <-    hop_count
        initial_time <-      time(NULL)
        sequence_numb... <-  sequence_number_destination
    */

    copy_addresses(item.aodv_destination, gm->aodv_destination);
    copy_addresses(item.next_hop, gm->source);
    item.number_of_hops = gm->hop_count;
    item.initial_time = time(NULL);
    item.sequence_number_destination = gm->sequence_number_destination;
  
    int index = get_index_item_routing_table(table, gm->aodv_destination);
    int route_changed = FALSE;

    if (index==-1)
    {
        put_item_at_routing_table(table, item);
        route_changed = TRUE;
        printf("Route added (it was not present in the table).\n");
    }
    else if (item.sequence_number_destination > table[index].sequence_number_destination)
    {  
        printf("Route item %d replaced (fresher data/sequence number of destination: %d > %d).\n", index, 
            item.sequence_number_destination, table[index].sequence_number_destination);
        put_item_at_routing_table(table, item);
        route_changed = TRUE;
    } 
    else if (item.sequence_number_destination == table[index].sequence_number_destination)
    {
        printf("Route item %d about to be replaced (same destination seq. number). Checking hops...\n", index);
        if (item.number_of_hops < table[index].number_of_hops)
        {
            printf("New route item %d has less number of hops(%d < %d). Replaced.\n", 
                index, item.number_of_hops, table[index].number_of_hops);
            put_item_at_routing_table(table, item);
            route_changed = TRUE;
        }
    }

    if (route_changed == FALSE)
    {
        printf("The routing table was NOT modified. ");
    }
}


void
use_RREQ_to_update_routing_table(routing_table_item table[], general_message* gm)
{
    
    printf("Updating routing table (way back) with RREQ... Using the following message: \n");
    print_general_message(gm); 

    routing_table_item item;
    /* 
    We receive a message that must be used to fill the table in this way:
        TABLE                Msg. RREQ            
        destination    <-    source_aodv
        next_hop <-          source (physical source)
        number_of_hops <-    hop_count
        initial_time <-      time(NULL) + (ROUTE_TIMEOUT / 10)*3 (only allow time for RREP message on the way back).
        sequence_numb... <-  0 (we want it not to persist since it's not necessary the best path). 
    */

    copy_addresses(item.aodv_destination, gm->aodv_source);
    copy_addresses(item.next_hop, gm->source);
    item.number_of_hops = gm->hop_count;
    item.initial_time = time(NULL) + (ROUTE_TIMEOUT / 10) * 3;
    item.sequence_number_destination = 0;
  
    int index = get_index_item_routing_table(table, gm->aodv_destination);
    int route_changed = FALSE;

    if (index==-1)
    {
        put_item_at_routing_table(table, item);
        route_changed = TRUE;
        printf("Route added (for way back) (it was not present in the table).\n");
    } 
    if (route_changed == FALSE)
    {
        printf("The routing table was NOT modified. ");
    }
}



void
assign_automatic_procedure(char* procedure, int cycle_counter)
{
    Sleep(1500);
    printf("\n\n   %d CYCLE\n\n", cycle_counter);

    int r = rand();

    if (r<(RAND_MAX/25))
    {
        *procedure = 's'; /* Send sometimes... */
    }
    else
    {
        *procedure = 'r'; /* Read each cycle... */                
    }

    if (cycle_counter%20==0)
    {
        *procedure = 'h'; /* Hello with frequence... */
    }

}

void
init_broadcast_table(int table[])
{
    int j;
    for(j=0;j<MAX_ELEMENTS_IN_REBROADCASTED_TABLE;j++)
    {
        table[j]=-1;            
    }
}

void
put_broadcast_sn(int table[], int sn, int* index)
{
    printf("Putting a new SN in the broadcast table.\n");
    table[*index] = sn;
    *index = *index + 1;
}


int
check_if_already_broadcasted(int table[], int sn, int index)
{
    int i;
    for (i=0;i<index;i++)
    {
        if (sn==table[i])
        {
            printf("Already broadcasted message (SN=%d).\n", sn);
            return TRUE;
        }
    }
    printf("Message not broadcasted before. Free to rebroadcast (SN=%d).\n", sn);
    return FALSE;
}

void
print_broadcasted_table(int table[], int index)
{
    int i;
    printf("\n**** BROADCASTED TABLE : ");
    for (i=0;i<index;i++)
    {
        printf("%d ", table[i]);
    }
    printf("\n\n");
}


int 
main()
{
    char buff[1024*8];
    int rebroadcast_index = 0;
    routing_table_item table[ROUTING_TABLE_MAX_ITEMS];
    
    initialize_routing_table(table);
    srand (time(NULL));
    int my_sequence_number = 0;

    int sn_of_broadcasted_messages_table[MAX_ELEMENTS_IN_REBROADCASTED_TABLE];
    init_broadcast_table(sn_of_broadcasted_messages_table);
    

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
        
    */    

    HANDLE fd;
    int MAX_CYCLES = 100;
    char data[] = "                                  ";

    address my_address, default_destination, broadcast;
    init_address(broadcast, "FFFF");
    printf("AODV/ZIGBEE\n---- ------\n\n");
    printf("Write the local address of one connected device (4 characters 0000-FFFF, for example '0001'): ");
    gets(buff);
    init_address(my_address, buff);
    init_address(default_destination, "0003");
    
    printf("Write serial port number (COM1-8, for example 'COM4'): ");
    gets(buff);
    initialize_zigbee_module(&fd, buff, my_address);

    if (fd!=(HANDLE)-1)
    {
        printf("Connected to ZigBee module.\n");

        int exit = FALSE;
        int mode_manual = TRUE;
        char procedure;
        int cycle_counter;

        while(exit==FALSE){
            int index;

            if (mode_manual==TRUE)
            {
                printf("\n\n*************\nInstructions: \n"
                "  c (Change destination address and message)\n"
                "  w (shoW routing table & rebroadcast table)\n"
                "  p (Proceed)\n" 
                "  r (Read incoming messages)\n" 
                "  s (Send data message to destination, changing destination \n    address before is required)\n"
                "  h (send Hello message)\n"
                "  t (test sending any raw message)\n"
                "  q (Quit)\n\n");
            
                procedure = getch();           
                fflush(stdin);  
            }
            else
            {
                printf("My sequence number %d. \n\n ", my_sequence_number);
                assign_automatic_procedure(&procedure, cycle_counter);

                if (cycle_counter%4==0)
                {   
                    print_routing_table(table);
                }
                cycle_counter++;

                if (cycle_counter > MAX_CYCLES)
                {
                    mode_manual = TRUE;
                }
            }



            switch(procedure)
            {
                case 'W':
                case 'w': // Show routing table.
                    print_routing_table(table);
                    print_broadcasted_table(sn_of_broadcasted_messages_table,rebroadcast_index);
                    break; 

                case 'R':
                case 'r': // Read. 
                {
                    read_one_message(fd, buff);
                    general_message* gm = (general_message*)buff; 
                    switch(buff[0])
                    {
                        /**************************/
                        /* Message RREQ received. */
                        /**************************/
                        case MESSAGE_ID_RREQ: 
                        {
                            int message_answered = FALSE;
                            printf("Message RREQ received: \n");
                            print_general_message(buff);

                            /* If the destination is me, reply RREP. */
                            if (addresses_are_equal(gm->aodv_destination, my_address))
                            {
                                message_answered = TRUE;
                                
                                send_general_message(fd, MESSAGE_ID_RREP, my_address, gm->source, 
                                    INITIAL_HOP_COUNT, gm->aodv_source, gm->aodv_destination, DEFAULT_TTL, 
                                    gm->sequence_number_message, ++my_sequence_number, NULL);
                            }
                            else if (index = get_index_item_routing_table(table, gm->aodv_destination)!=-1)
                            { /* If the destination is in my link state table, reply RREP. */
                                if (table[index].number_of_hops==1) /* It's a neighbor. */
                                {
                                    message_answered = TRUE;
                                    send_general_message(fd, MESSAGE_ID_RREP, my_address, gm->source, 
                                        INITIAL_HOP_COUNT, gm->aodv_source, gm->aodv_destination, DEFAULT_TTL, gm->sequence_number_message, 
                                        table[index].sequence_number_destination, NULL);
                                }
                            }
                            
                            if (message_answered == FALSE) /* No information about the requested node. */
                            {
                                if ((gm->lifespan > gm->hop_count) && (!check_if_already_broadcasted(sn_of_broadcasted_messages_table, gm->sequence_number_message, rebroadcast_index)))
                                /* If it's still valid and was not broadcasted before, forward RREQ. */
                                {
                                    put_broadcast_sn(sn_of_broadcasted_messages_table, gm->sequence_number_message, &rebroadcast_index);
                        
                                    use_RREQ_to_update_routing_table(table, gm);
                                    send_general_message(fd, MESSAGE_ID_RREQ, my_address, broadcast, 
                                        gm->hop_count+1, gm->aodv_source, gm->aodv_destination, gm->lifespan, 
                                        gm->sequence_number_message, gm->sequence_number_destination, NULL);
                                    printf("RREQ forwarded.\n");

                                }
                                else
                                {
                                    printf("RREQ not forwarded ( already broadcasted or hopcount=%d>=lifespan=%d).\n", gm->hop_count, gm->lifespan);
                                }
                            }
                            break;
                        }    

                        /**************************/
                        /* Message RREP received. */
                        /**************************/
                        case MESSAGE_ID_RREP:
                        {
                            int index; 
                            use_RREP_to_update_routing_table(table, gm);
                            if (!addresses_are_equal(gm->aodv_destination, my_address))
                            {
                                if ((index=get_index_item_routing_table(table, gm->aodv_destination))!=-1)
                                {
                                    printf("RREP forwarded...\n");
                                    send_general_message(fd, MESSAGE_ID_RREP, my_address, table[index].next_hop, 
                                        gm->hop_count+1, gm->aodv_source, gm->aodv_destination, gm->lifespan, 
                                        gm->sequence_number_message, gm->sequence_number_destination, NULL);
                                }
                                else
                                {
                                    printf("RREQ cannot be forwarded. Error in routing.\n");
                                }
                            }
                            else
                            {
                                printf("RREP message used to update routing table. Not forwarded.\n");
                            }
                            
                            break;
                        }

                        /**************************/
                        /* Message HELLO received. */
                        /**************************/
                        case MESSAGE_ID_HELL:
                        {
                            use_HELLO_to_update_routing_table(table, gm);
                            break;
                        }
    
                        /**************************/
                        /* Message DATA received. */
                        /**************************/
                        case MESSAGE_ID_DATA:
                        {
                            if (addresses_are_equal(gm->aodv_destination, my_address))
                            {
                                printf("Data message for me received!!!\n"); 
                                print_general_message(buff);
                            }
                            else
                            {
                                printf("Data message NOT for me, received.\n");  
                                print_general_message(buff);
                                int index;
                                if ((index=get_index_item_routing_table(table, gm->aodv_destination))!=-1)
                                {
                                    if (gm->hop_count < gm->lifespan)
                                    {
                                        printf("Route known. Forwarding...\n");
                                        send_general_message(fd, MESSAGE_ID_DATA, my_address, table[index].next_hop, 
                                          gm->hop_count+1, gm->aodv_source, gm->aodv_destination, gm->lifespan, 
                                            gm->sequence_number_message, gm->sequence_number_destination, gm->data);
                                    }
                                    else
                                    {
                                        printf("Dropping packet (lifespan expired). ");
                                    }
                                }
                                else
                                {
                                    printf("Route unknown. Dropping packet.\n");
                                }
                            }
                            
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case 'C': // Change data/destination.
                case 'c': // Change data/destination.
                    printf("Please, write the new destination address (4 characters, 0000-FFFF, for example '0002'): "); 
                    gets(buff);
                    init_address(default_destination, buff);
                    printf("Please, write the new data (less than 20 characters): "); 
                    gets(data);
                    break;
            

                case 'S': // Send.
                case 's': // Send.
                    printf("Trying to send a message...\n");
                    
                    int route_found = FALSE;
                    
                    if ((index = get_index_item_routing_table(table, default_destination))!=-1)
                    {
                        /* Send Message (using the next hop). */
                        printf("Destination is already in the routing table.\n");
                        
                        /* It's okay. We can use this route now. */
                        
                        route_found = TRUE;
                        printf("Route found!\n");
                            
                    }
                    
                    if (route_found == FALSE)
                    {
                        general_message* mes = (general_message*)buff;
                        int times = 3;
                        while(TRUE)
                        {
                            /* Send RREQ to discover a new route. */
                            printf("Destination is NOT in the table. Sending RREQ...\n");
                            int message_sequence_number = rand();
                            send_general_message(fd, MESSAGE_ID_RREQ, my_address, broadcast, 
                                                INITIAL_HOP_COUNT, my_address,  /* Looking for ... */ default_destination , 
                                                DEFAULT_TTL, message_sequence_number, 0,NULL);
                            put_broadcast_sn(sn_of_broadcasted_messages_table, message_sequence_number, &rebroadcast_index);
                            /* Wait for the first RREP. The other RREP messages will 
                            be processed in other stage. */
                            printf("Waiting for the first RREP...\n"); 
                            Sleep(7000); 

                            read_one_message(fd, buff);
                            if (buff[0] == MESSAGE_ID_RREP) /* The expected message was received. */
                                break;
                            if (--times <0)
                                break;
                        }
                    
                        if (times<0)
                        {
                            printf("Aborting the send of a message. The destination either does not exist, or is busy.\n");
                        }
                        else
                        {
                            use_RREP_to_update_routing_table(table, mes);
                            route_found = TRUE;
                        }
                    }

                    if (route_found == TRUE)
                    {
                        index = get_index_item_routing_table(table, default_destination);
                        printf("Route available (routing table item = %d). Now sending message...\n", index);
                        int message_sequence_number = rand();
                        send_general_message(fd, MESSAGE_ID_DATA, my_address, table[index].next_hop, 
                                INITIAL_HOP_COUNT, my_address, default_destination, DEFAULT_TTL, message_sequence_number, 0, data);
                        
                    }
                    break;

                case 'H':
                case 'h':
                    printf("Sending HELLO message: \n");
                    send_hello_message(fd, my_address, &my_sequence_number);
                    break;

                case 'Q':
                case 'q': // Quit. 
                    exit = TRUE;
                    break;
                case 'p':
                case 'P':
                    printf("Please, write the new destination address (4 characters, 0000-FFFF, for example '009F'): "); 
                    gets(buff);
                    init_address(default_destination, buff);
                    printf("Please, write the new data (less than 20 characters): "); 
                    gets(data);
            
                    mode_manual = (mode_manual?FALSE:TRUE);
                    printf("Write the amount of cycles to run automatically: ");
                    scanf("%d", &MAX_CYCLES);
                    cycle_counter = 0;
                    break;
                case 't':
                case 'T':
                    {
                        int ttll=1, mseqn=18, sn_destin, hcn;
                        char mesid = MESSAGE_ID_DATA;
                        char bf[6];
                        address nh, aodv_dest, aodv_src; 
                        printf("Testing...\n");
                        printf("Write message ID (RREQ 'Q', RREP 'P', HELL 'H', DATA 'D' UPPERCASE): \n");
                        mesid = getche();
                        printf("\nWrite next_hop: \n");
                        gets(bf);
                        init_address(nh, bf);
                        printf("Write aodv_destination: \n");
                        gets(bf);                        
                        init_address(aodv_dest, bf); 
                        printf("Write aodv_source: \n");
                        gets(bf);                        
                        init_address(aodv_src, bf); 
                        printf("Write lifespan:\n");
                        scanf("%d", &ttll);
                        printf("Write hop_count:\n");
                        scanf("%d", &hcn);
                        printf("Write SN of destination.\n");
                        scanf("%d", &sn_destin);
                        printf("Testing...\n");
                        send_general_message(fd, mesid, my_address, nh, 
                                    INITIAL_HOP_COUNT, aodv_src, aodv_dest, ttll, mseqn, sn_destin, NULL);
                    }
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

