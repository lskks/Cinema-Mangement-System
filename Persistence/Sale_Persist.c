#include "Sale_Persist.h"
#include "../Service/sale.h"
#include "EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
//#include<unistd.h>
#include <assert.h>
#include <string.h>

#define SALE_FILE "Sale.dat"
#define TICKET_FILE "Ticket.dat"

int Sale_Perst_Insert(const sale_t* data) {
    if (data == NULL) {
        return -1; 
    }

    long new_id = EntKey_Perst_GetNewKeys("sale", 1);
    if (new_id < 0) {
        return -1; 
    }

    sale_t new_data = *data;
    new_data.id = new_id;

    FILE* fp = fopen(SALE_FILE, "ab");
    if (fp == NULL) {
        perror("open fauls");
        return -1;
    }

    size_t write_size = fwrite(&new_data, sizeof(sale_t), 1, fp);
    int rtn = (write_size == 1) ? 0 : -4;

    fclose(fp);
    return rtn;
}

int Ticket_Perst_Update(const ticket_t* data) {
    if (data == NULL || data->id <= 0) {
        return -1; 
    }

    FILE* fp = fopen(TICKET_FILE, "rb+");
    if (fp == NULL) {
        perror("打开票文件失败");
        return -1;
    }

    int rtn = 0;
    ticket_t rec;
    long pos;

    while (fread(&rec, sizeof(ticket_t), 1, fp) == 1) {
        if (rec.id == data->id) {
            pos = ftell(fp);
            fseek(fp, pos - sizeof(ticket_t), SEEK_SET);
            size_t write_size = fwrite(data, sizeof(ticket_t), 1, fp);
            if (write_size == 1) {
                rtn = 1; 
            }
            else {
                rtn = -1; 
            }
            break;
        }
    }

    fclose(fp);
    return rtn;
}

int Ticket_Srv_SelBySchID(int id, ticket_list_t* list) {

    *list = NULL;
    int count = 0;
    ticket_node_t* tail = NULL; 

    FILE* fp = fopen(TICKET_FILE, "rb");
    if (fp == NULL) {
        perror("打开票文件失败");
        return -1;
    }

    ticket_t data;
    while (fread(&data, sizeof(ticket_t), 1, fp) == 1) {
        if (data.schedule_id == id) {
            ticket_node_t* new_node = (ticket_node_t*)malloc(sizeof(ticket_node_t));
            if (new_node == NULL) {
                perror("内存分配失败");
                fclose(fp);
                return -2;
            }
            new_node->data = data;
            new_node->next = NULL;
            new_node->prev = tail;
            if (*list == NULL) {
                *list = new_node; 
            }
            else {
                tail->next = new_node; 
            }
            tail = new_node; 
            count++; 
        }
    }

    fclose(fp);
    return count;
}