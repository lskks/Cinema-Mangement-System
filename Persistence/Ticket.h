#ifndef TICKET_PERSIST_H_
#define TICKET_PERSIST_H_

#include "../Service/Ticket.h"

// TTMS_SCU_Ticket_Perst_Insert - 存储演出票（批量）
int Ticket_Perst_Insert(ticket_list_t list, int schedule_id);

// TTMS_SCU_Ticket_Perst_Rem - 根据演出计划ID去除演出票
int Ticket_Perst_Rem(int schedule_id);

#endif /* TICKET_PERSIST_H_ */