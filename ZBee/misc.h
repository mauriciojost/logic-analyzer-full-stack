#ifndef MISC_H
#define MISC_H

#define TRUE 1 
#define FALSE 0

#define ROUTING_TABLE_MAX_ITEMS 8

typedef char address [4];

typedef struct{
    char empty;
    address destination;
    address nexthop;
    int sequence_number;
    int number_of_hops;
} routing_table_item;

typedef struct{
    address destination; 
    address source;
    int lifespan;
    int id;
} RREQ;

typedef struct{
    
} RREP;


void initialize_routing_table(routing_table_item table[]);
int addresses_are_equal(address a, address b);
void copy_addresses(address dest, address sou);
void put_item_at_routing_table(routing_table_item table[], routing_table_item item);
int get_index_item_routing_table(routing_table_item table[], address destination);
void remove_item_routing_table(routing_table_item table[], address destination);
void print_routing_table_item(routing_table_item item);
void print_routing_table(routing_table_item table[]);
void init_address(address a, char* str);


#endif /* MISC_H */
