#ifndef TTMS_TICKET_PERSIST_H
#define TTMS_TICKET_PERSIST_H

#include "../Service/Ticket.h"

int Ticket_Perst_SelByID(int id, ticket_t * buf);

int Ticket_Perst_Update(const ticket_t *data);


#endif