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
        table[i].empty = TRUE;
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
put_item_at_routing_table(routing_table_item table[], routing_table_item item)
{   
    int i;
    printf("Put item...\n");

    remove_item_routing_table(table, item.destination);

    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if (table[i].empty == TRUE)
        {
            break;
        }
    }
    
    if (i<ROUTING_TABLE_MAX_ITEMS)
    {
        table[i].empty = FALSE;
        copy_addresses(table[i].destination, item.destination);
        copy_addresses(table[i].nexthop, item.nexthop);
        table[i].sequence_number = item.sequence_number;
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
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if (addresses_are_equal(table[i].destination, destination) 
            && (table[i].empty==FALSE))
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
    i = get_index_item_routing_table(table, destination);
    table[i].empty = TRUE;
}
    
void
print_routing_table_item(routing_table_item item)
{

    printf("  dest=[%c%c%c%c] empty=[%d]\n" , 
                                    item.destination[0], 
                                    item.destination[1], 
                                    item.destination[2], 
                                    item.destination[3], 
                                    item.empty);
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

