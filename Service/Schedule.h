#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/common.h"

// 演出计划数据结构
typedef struct {
    int id;                 // 演出计划ID
    int play_id;            // 剧目ID
    int studio_id;          // 演出厅ID
    user_date_t date;       // 演出日期
    user_time_t time;       // 开始时间
    int seat_count;         // 已售票数/座位总数
    float price;            // 票价
    int status;             // 状态：0-未开始，1-已开始，2-已结束
} schedule_t;

// 双向链表
typedef struct schedule_node {
    schedule_t data;
    struct schedule_node *next, *prev;
} schedule_node_t, *schedule_list_t;

// 标识符：TTMS_SCU_Schedule_Srv_FetchAll
// 功能：获取全部演出计划服务
int Schedule_Srv_FetchAll(schedule_list_t list);

// 标识符：TTMS_SCU_Schedule_Srv_FetchByPlay
// 功能：根据剧目ID获取演出计划服务
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);

int Schedule_Srv_FetchByID(int id, schedule_t *buf);

int Schedule_Srv_Add(schedule_t *data);

int Schedule_Srv_Modify(schedule_t *data);

int Schedule_Srv_DeleteByID(int id);

// 功能：统计指定剧目的售票数量与票房
// 返回值：总票房；soldCount返回总售票数量
float Schedule_Srv_StatRevByPlay(int play_id, int *soldCount);

#endif /* SCHEDULE_H_ */
