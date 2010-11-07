#ifndef MISC_H
#define MISC_H

#include <time.h>

#define TRUE 1 
#define FALSE 0

#define INITIAL_HOP_COUNT 0
#define DEFAULT_TTL 10
#define ROUTE_TIMEOUT 1000000

#define DATA_FIELD_LENGTH 8
#define MESSAGE_ID_INVALID 'x'
#define MESSAGE_ID_RREQ 'Q'
#define MESSAGE_ID_RREP 'P'
#define MESSAGE_ID_HELL 'H'
#define MESSAGE_ID_DATA 'D'

#define ROUTING_TABLE_MAX_ITEMS 8

typedef char address [4]; /* Type 'address'. */

typedef struct{
	/***********************/
	/* ROUTING TABLE ITEMS */
	/***********************/
	
    char is_item_empty;
    address aodv_destination;
    address next_hop;
    int number_of_hops; /* Cost. */
    time_t initial_time; /* Lifetime. */
    int sequence_number_destination;
} routing_table_item;




typedef struct{
	char message_id; 			/* Identifier of the type of message: RREQ, RREP, HELL... */
	
	/******************************/
	/* REAL FIELDS OF THE MESSAGE */
	/******************************/
	address source; 			/* Physical source. 								*/
    address destination; 		/* Physical destination. 							*/
    int hop_count; 				/* Hops already visited. 							*/
    address aodv_source; 		/* Logical source. 									*/
    address aodv_destination;	/* Logical destination. 							*/
    int lifespan; 				/* TTL (uses hop_count to check its trip). 			*/
    int sequence_number_message;/* Identifier of this particular message's instance 
									(used to avoid re-broadcast). 					*/
	int sequence_number_destination; 
								/* Used to see how fresh is the information about the destination. */
	char data[DATA_FIELD_LENGTH];/* Payload. 										*/
	
	/* Not used in this implementation.
	int sequence_number_source;
	int sequence_number_destionation;
	*/
	char tail; /* Must be 0 (it's useful to print the whole message at once). */
} general_message;



void initialize_routing_table(routing_table_item table[]);
int addresses_are_equal(address a, address b);
void copy_addresses(address dest, address sou);
void put_item_at_routing_table(routing_table_item table[], routing_table_item item);
int get_index_item_routing_table(routing_table_item table[], address destination);
void remove_item_routing_table(routing_table_item table[], address destination);
void print_routing_table_item(routing_table_item item);
void print_routing_table(routing_table_item table[]);
void init_address(address a, char* str);
void print_address(address ad);

#endif /* MISC_H */
