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
put_item_at_routing_table(routing_table_item table[], routing_table_item item)
{   
    int i;
    printf("Put item...\n");

    remove_item_routing_table(table, item.destination);

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
        copy_addresses(table[i].destination, item.destination);
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
    int i;
    for (i=0;i<ROUTING_TABLE_MAX_ITEMS;i++)
    {
        if (addresses_are_equal(table[i].destination, destination) 
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
    i = get_index_item_routing_table(table, destination);
    table[i].is_item_empty = TRUE;
}
    
void
print_routing_table_item(routing_table_item item)
{
    printf("  dest=["); print_address(item.destination); printf("] empty=[%d]\n", item.is_item_empty);
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
