#include "Ticket.h"
#include "../Common/List.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Service/Schedule.h"
#include "Sale_Persist.h"
#include "Seat.h"
#include "Ticket.h"
#include <assert.h>
#include <stdlib.h>

int Ticket_Srv_GenBatch(int schedule_id)
{
    int count = 0;
    seat_list_t seat_head;

    List_Init(seat_head, seat_node_t);

    schedule_t buf;

    Schedule_Srv_FetchByID(schedule_id, &buf);
    count = Seat_Srv_FetchValidByRoomID(seat_head, buf.studio_id);
    Ticket_Perst_Insert(seat_head, schedule_id);

    return count;
}

void Ticket_Srv_DeleteBatch(int schedule_id)
{
    Ticket_Perst_Rem(schedule_id);
}

int Ticket_Srv_FetchAll(ticket_list_t list)
{
    return Ticket_Perst_SelectAll(list);
}

int Ticket_Srv_FetchByTicketID(int ticket_id, ticket_t *buf)
{
    return Ticket_Perst_SelByticketID(ticket_id, buf);
}

int Ticket_Srv_FetchByID(int id, ticket_t *buf)
{
    return Ticket_Perst_SelByticketID(id, buf);
}

int Ticket_Srv_Modify(const ticket_t *data)
{
    return Tickect_Perst_Update(data);
}

int Ticket_Srv_FetchBySchID(int id, ticket_list_t list)
{
    return Ticket_Perst_SelBySchID(id, list);
}
