#include "Sale.h"
#include "Sale_Persist.h"
#include "../Common/List.h"

int Sale_Srv_Add(sale_t* data)
{
    return Sale_Perst_Insert(data);
}

int Sale_Srv_Modify(const ticket_t* data)
{
    return Tickect_Perst_Update(data);
}

int Sale_Srv_FetchBySchID(int id, ticket_list_t list)
{
    return Ticket_Persit_SelBySchedule(id, list);
}

ticket_node_t* Sale_Srv_FetchBySeatID(int seat_id, ticket_list_t list)
{
    ticket_node_t* pos;
    List_ForEach(list, pos)
    {
        if (pos->data.id == seat_id)
            return pos;
    }
    return NULL;
}

int Sale_Srv_FetchAll(sale_list_t list)
{
    return Sale_Perst_SeclectAll(list);
}