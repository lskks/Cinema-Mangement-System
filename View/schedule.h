//
// Created by wjw on 2026/3/19.
//


#ifndef SCHEDULE_UI_H_
#define SCHEDULE_UI_H_

//标识符：TTMS_SCU_Schedule_UI_List
//功能：查询演出界面
void Schedule_UI_ListAll(void);

//标识符：TTMS_SCU_Schedule_UI_Qry
//功能：根据剧目名称获取演出计划
int Schedule_UI_Query(char *play_name);

#endif /* SCHEDULE_UI_H_ */
