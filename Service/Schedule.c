
#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "List.h"
#include <stdlib.h>
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
