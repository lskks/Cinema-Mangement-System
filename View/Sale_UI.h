#ifndef Sale_UI_H_
#define Sale_UI_H_
#include "../Service/Play.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"

void Sale_UI_MgtEntry(void);

int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList);

void Sale_UI_ShowTicket(int scheduleID);

void Sale_UI_ShowScheduler(int playID);

#endif