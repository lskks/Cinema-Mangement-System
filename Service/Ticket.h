#ifndef TICKET_H_
#define TICKET_H_
#include"Schedule.h"

typedef enum
{
	TICKET_AVL = 0,   
	TICKET_SOLD = 1,
	TICKET_RETURN = 2,
	TICKET_RESV = 9 
}ticket_status_t;

typedef struct 
{
	int id;
	int schedule_id;
	int seat_id;
	int price; 
	ticket_status_t status;
}ticket_t;

typedef struct ticket_node
{
	ticket_t data;
	struct ticket_node *next,*prev;
}ticket_node_t,* ticket_list_t;

int Ticket_Srv_GenBatch(int schedule_id);       

void Ticket_Srv_DeleteBatch(int schedule_id); 

int  Ticket_Srv_FetchAll(ticket_list_t list);

int Ticket_Srv_FetchByTicketID(int ticket_id,ticket_t *buf);        

int Ticket_Srv_FetchBySchID(int id, ticket_list_t list);

int Ticket_Srv_FetchByID(int id,ticket_t *buf);

#endif
