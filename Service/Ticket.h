#ifndef TICKET_H_
#define TICKET_H_

#define TICKET_DATA_FILE "ticket.dat"
#define TICKET_DATA_TEMP_FILE "ticket.tmp"

typedef enum
{
    TICKET_AVL = 0,
    TICKET_SOLD = 1,
    TICKET_RESV = 9,
} ticket_status_t;

typedef struct
{
    int id;
    int schedule_id;
    int seat_id;
    int price;
    ticket_status_t status;
} ticket_t;

typedef struct ticket_node 
{
    ticket_t data;
    struct ticket_node* prev;
    struct ticket_node* next;
} ticket_node_t, *ticket_list_t;

#endif