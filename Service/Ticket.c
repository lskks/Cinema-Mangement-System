#include "Ticket.h"
#include "../Common/List.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Service/Schedule.h"
#include "../Service/Studio.h"
#include "Sale_Persist.h"
#include "Seat.h"
#include "Ticket.h"
#include <assert.h>
#include <stdlib.h>

int Ticket_Srv_GenBatch(int schedule_id)
{
    int count = 0;
    int inserted = 0;
    seat_list_t seat_head;
    studio_t studio;

    List_Init(seat_head, seat_node_t);

    schedule_t buf;

    if (!Schedule_Srv_FetchByID(schedule_id, &buf))
    {
        return 0;
    }
    count = Seat_Srv_FetchValidByRoomID(seat_head, buf.studio_id);
    if (count <= 0)
    {
        if (Studio_Srv_FetchByID(buf.studio_id, &studio))
        {
            List_Free(seat_head, seat_node_t);
            if (Seat_Srv_RoomInit(seat_head, buf.studio_id, studio.rowsCount, studio.colsCount) >
                    0 &&
                Seat_Srv_AddBatch(seat_head) > 0)
            {
                List_Free(seat_head, seat_node_t);
                count = Seat_Srv_FetchValidByRoomID(seat_head, buf.studio_id);
            }
        }
        if (count <= 0)
        {
            return 0;
        }
    }

    inserted = Ticket_Perst_Insert(seat_head, schedule_id);
    if (inserted <= 0)
    {
        fprintf(stderr, "Failed to generate tickets for schedule ID %d\n", schedule_id);
        return 0;
    }

    return inserted;
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
