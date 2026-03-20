
#include "Schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include <stdlib.h>
#include <assert.h>

int Schedule_Srv_FetchAll(schedule_list_t list) {
    assert(NULL != list);
    return Schedule_Perst_SelectAll(list);
}

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id) {
    assert(NULL != list);
    return Schedule_Perst_SelectByPlay(list, play_id);
}