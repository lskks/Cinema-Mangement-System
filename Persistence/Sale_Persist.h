#ifndef SALE_PERSIST_H_
#define SALE_PERSIST_H_

#include "../Service/Sale.h"
#include "../Service/Ticket.h"

#define SALE_DATA_FILE "sale.dat"
#define SALE_DATA_TEMP_FILE "sale.tmp"

int Sale_Perst_Insert(sale_t* data);

int Tickect_Perst_Update(const ticket_t* data);

int Ticket_Srv_SelBySchedule(int id, ticket_list_t list);

int Sale_Perst_SeclectAll(sale_list_t list);

#endif