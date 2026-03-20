#include "Studio.h"
#include "../Persistence/Sale_Persist.h"
#include <stdlib.h>
#include <assert.h>

int Sale_Srv_Add(const sale_t* data) {
    return Sale_Perst_Insert(data);
}

int Ticket_Srv_Modify(const ticket_t* data) {
    return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchBySchID(int id, ticket_list_t* list) {
    return Ticket_Srv_SelBySchID(id, list);
}

ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id) {
    if (list == NULL) {
        return NULL;
    }

    ticket_node_t* curr = list;
    while (curr != NULL) {
        if (curr->data.seat_id == seat_id) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL; 
}