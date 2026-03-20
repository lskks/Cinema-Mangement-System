
#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "List.h"
#include "Ticket.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int Schedule_Srv_FetchAll(schedule_list_t list) {
    assert(NULL != list);
    return Schedule_Perst_SelectAll(list);
}

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id) {
    assert(NULL != list);
    return Schedule_Perst_SelectByID(list, play_id);
}

int Schedule_Srv_FetchByID(int id, schedule_t *buf)
{
    assert(NULL != buf);
    schedule_list_t list;
    schedule_node_t *pos;
    int found = 0;

    List_Init(list, schedule_node_t);
    Schedule_Perst_SelectAll(list);

    List_ForEach(list, pos) {
        if (pos->data.id == id) {
            *buf = pos->data;
            found = 1;
            break;
        }
    }

    List_Destroy(list, schedule_node_t);
    return found;
}


int Schedule_Srv_Add(schedule_t *data)
{
    assert(NULL != data);
    return Schedule_Perst_Insert(data);
}

int Schedule_Srv_Modify(schedule_t *data)
{
    assert(NULL != data);
    return Schedule_Perst_Modify(data);
}

int Schedule_Srv_DeleteByID(int id)
{
    return Schedule_Perst_DeleteByID(id);
}

float Schedule_Srv_StatRevByPlay(int play_id, int *soldCount)
{
    schedule_list_t schedule_list;
    schedule_node_t *sch_pos;
    ticket_list_t ticket_list;
    ticket_node_t *tk_pos;
    int sold = 0;
    float total = 0.0f;

    assert(NULL != soldCount);
    *soldCount = 0;

    List_Init(schedule_list, schedule_node_t);
    List_Init(ticket_list, ticket_node_t);

    Schedule_Srv_FetchByPlay(schedule_list, play_id);
    Ticket_Srv_FetchAll(ticket_list);

    List_ForEach(schedule_list, sch_pos) {
        List_ForEach(ticket_list, tk_pos) {
            if (tk_pos->data.schedule_id == sch_pos->data.id &&
                tk_pos->data.status == TICKET_SOLD) {
                sold++;
                total += (float)tk_pos->data.price;
            }
        }
    }

    List_Destroy(schedule_list, schedule_node_t);
    List_Destroy(ticket_list, ticket_node_t);

    *soldCount = sold;
    return total;
}
