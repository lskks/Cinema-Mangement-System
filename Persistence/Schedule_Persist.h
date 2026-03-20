
#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include "../Service/Schedule.h"

// 标识符：TTMS_SCU_Schedule_Perst_SelAll
// 功能：载入全部演出计划
int Schedule_Perst_SelectAll(schedule_list_t list);

// 标识符：TTMS_SCU_Schedule_Perst_SelByPlay
// 功能：根据剧目ID载入演出计划
int Schedule_Perst_SelectByID(schedule_list_t list, int play_id);


#endif /* SCHEDULE_PERSIST_H_ */