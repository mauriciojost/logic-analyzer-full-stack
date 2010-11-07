#include "misc.h"
#include <stdio.h>
#include <string.h>

/***********************/
/* ROUTING TABLE STUFF */
/***********************/

void 
initialize_routing_table(routing_table_item table[])
{
    /* Initialize */
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        table[i].is_item_empty = TRUE;
    }
}

int 
addresses_are_equal(address a, address b)
{
    return (a[0]==b[0] && a[1]==b[1] && a[2]==b[2] && a[3]==b[3]);
}

void
copy_addresses(address dest, address sou)
{
    dest[0] = sou[0];
    dest[1] = sou[1];
    dest[2] = sou[2];
    dest[3] = sou[3];
}

void
init_address(address a, char* str)
{
    if (strlen(str)==4)
    {
        int i;
        for (i=0; i<4; i++)
            a[i] = str[i];
    }
    else
    {
        printf("ERROR: Invalid address.\n");
    }
}

void
remove_old_items(routing_table_item table[])
{
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if ((time(NULL) - table[i].initial_time) > ROUTE_TIMEOUT)
        {
            printf("Detected obsolete route in routing table. Removing... \n");
            table[i].is_item_empty = TRUE;
        }
    }
}

void
put_item_at_routing_table(routing_table_item table[], routing_table_item item)
{   
    int i;
    printf("Put item...\n");
    remove_old_items(table);
    remove_item_routing_table(table, item.aodv_destination);

    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if (table[i].is_item_empty == TRUE)
        {
            break;
        }
    }
    
    if (i<ROUTING_TABLE_MAX_ITEMS)
    {
        table[i].is_item_empty = FALSE;
        copy_addresses(table[i].aodv_destination, item.aodv_destination);
        copy_addresses(table[i].next_hop, item.next_hop);
        table[i].initial_time = item.initial_time; 
        table[i].sequence_number_destination = item.sequence_number_destination;
        table[i].number_of_hops = item.number_of_hops;
    }
    else
    {
        printf("ERROR: Maximun number of items on a routing table reached...\n");
    }
}


int
get_index_item_routing_table(routing_table_item table[], address destination)
{
    printf("Get index item...\n");
    remove_old_items(table);
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if (addresses_are_equal(table[i].aodv_destination, destination) 
            && (table[i].is_item_empty==FALSE))
        {
            break;
        }
    }

    i = (i==ROUTING_TABLE_MAX_ITEMS?-1:i);

    return i;
}

void
remove_item_routing_table(routing_table_item table[], address destination)
{
    printf("Remove item...\n");
    int i;

    remove_old_items(table);

    i = get_index_item_routing_table(table, destination);
    table[i].is_item_empty = TRUE;
}
    
void
print_routing_table_item(routing_table_item item)
{
    if (item.is_item_empty)
    {
        printf("Empty=[%d]\n\n"); 
    }
    else
    {
        printf("Empty=[%d] dest=[\n", item.is_item_empty); 
        print_address(item.aodv_destination); 
        printf("] next_hop=["); 
        print_address(item.next_hop); 
        printf("] NoHops=%d in_time=%ld sn_dest=%d\n", 
            item.number_of_hops, item.initial_time, item.sequence_number_destination); 
    }
    
}

void
print_routing_table(routing_table_item table[])
{
    printf("Table...\n");    
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {   
        print_routing_table_item(table[i]);
    }
    printf("End table...\n");    
}

void 
print_address(address ad)
{
	printf("%c%c%c%c", ad[0], ad[1], ad[2], ad[3]);
}
