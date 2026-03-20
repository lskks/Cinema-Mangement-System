#include "../Common/List.h"
#include <stdio.h>

#include "Sale_Persist.h"
#include "EntityKey_Persist.h"

int Sale_Perst_Insert(sale_t* data)
{
    long key = EntKey_Perst_GetNewKeys("sale", 1);
    FILE* fp = fopen(SALE_DATA_FILE, "ab");

    if (key <= 0)
    {
        fprintf(stderr, "allocate the key failed");
        return -1;
    }

    data->id = key;

    if (fp == NULL)
    {
        fprintf(stderr, "open file %s failed", SALE_DATA_FILE);
        return -1;
    }

    int rtn = fwrite(data, sizeof(sale_t), 1,fp);

    return rtn;
}

int Tickect_Perst_Update(const ticket_t* data)
{
    ticket_t rec;
    int rtn = -1;

    FILE* fp = fopen(TICKET_DATA_FILE, "rb+");

    if (fp == NULL)
    {
        fprintf(stderr, "open file %s failed", TICKET_DATA_FILE);
        return -1;
    }

    while (fread(&rec, sizeof(ticket_t), 1, fp) == 1)
    {
        if (rec.id == data->id)
        {
            fseek(fp, -((int)sizeof(ticket_t)), SEEK_CUR);
            fwrite(data, sizeof(ticket_t), 1, fp);
            rtn = 1;
            break;
        }
    }

    fprintf(stderr, "ticket not found");
    fclose(fp);
    return rtn;
}

int Ticket_Srv_SelBySchedule(int id, ticket_list_t list)
{
    int count = 0;
    List_Init(list, ticket_node_t);
    ticket_t* data;

    FILE* fp = fopen(TICKET_DATA_FILE, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "open file %s failed", TICKET_DATA_FILE);
        return -1;
    }

    while (fread(data, sizeof(ticket_t), 1, fp) == 1)
    {
        if (data->schedule_id == id)
        {
            ticket_node_t* newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
            if (!newNode)
            {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = *data;
            List_AddTail(list, newNode);
            count++;
        }
    }
    fclose(fp);
    return count;
}