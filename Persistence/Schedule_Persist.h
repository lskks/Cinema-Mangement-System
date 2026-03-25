
#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include "../Service/Schedule.h"

int Schedule_Perst_SelectAll(schedule_list_t list);

int Schedule_Perst_SelectByID(schedule_list_t list, int play_id);

int Schedule_Perst_Insert(schedule_t *data);

int Schedule_Perst_Modify(schedule_t *data);

int Schedule_Perst_DeleteByID(int id);

void Schedule_Perst_SelectByIDForSchedule(schedule_t *buf, int id);

#endif /* SCHEDULE_PERSIST_H_ */