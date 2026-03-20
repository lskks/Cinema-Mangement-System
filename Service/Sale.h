#ifndef SALE_H_
#define SALE_H_

#include "../Common/common.h"
#include "Ticket.h"

typedef enum 
{
    SALE_SELL = 0,
    SALE_REFOUND = 1
} sale_type_t;

typedef struct
{
    long id;
    int user_id;
    int ticket_id;
    user_date_t date;
    user_time_t time;
    int value;              // the price of ticket
    sale_type_t type;
} sale_t;


typedef struct sale_node
{
    sale_t data;
    struct sale_node* prev;
    struct sale_node* next;
} sale_node_t, *sale_list_t;

int Sale_Srv_Add(sale_t* data);

int Sale_Srv_Modify(const ticket_t* data);

int Sale_Srv_FetchBySchID(int id, ticket_list_t list);

ticket_node_t* Sale_Srv_FetchBySeatID(int seat_id, ticket_list_t list);

int Sale_Srv_FetchAll(sale_list_t list);

#endif /* SALE_H_ */
