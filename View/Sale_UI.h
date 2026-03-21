#ifndef SEAT_UI_H_
#define SEAT_UI_H_

#include "../Service/Sale.h"
#include "../Service/Seat.h"

#define SALE_PAGE_SIZE 5

void Sale_UI_MgtEntry();

void Sale_UI_ShowSheduler(int playID);

int Sale_UI_Sell_Ticket(ticket_list_t ticket_list, seat_list_t seat_list);

void Sale_UI_RefundTicket();

#endif