#include "Ticket.h"
#include "Schedule.h"
#include "Play.h"
#include "Seat.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int Ticket_Srv_DeleteBatch(int schedule_id) {
    printf("业务逻辑层：开始删除演出计划ID=%d的票\n", schedule_id);
    // a) 调用持久化层根据演出计划ID去除票函数
    int found = Ticket_Perst_Rem(schedule_id);
    // b) 返回found
    return found;
}
int Ticket_Srv_GenBatch(int schedule_id) {
    seat_list_t seat_head = NULL;
    schedule_t schedule;
    int count = 0;
    int rtn;

    printf("业务逻辑层：开始批量生成演出票，schedule_id=%d\n", schedule_id);

    // 1. 调用List_Init函数初始化座位信息链表
    List_Init(seat_head, seat_node_t);

    // 获取演出计划信息（用于获取演出厅ID）
    rtn = Schedule_Srv_FetchByID(schedule_id, &schedule);
    if (rtn != 1) {
        printf("业务逻辑层：获取演出计划失败\n");
        return -1;
    }

    // 2. 调用业务逻辑层根据演出厅ID获取有效座位服务函数
    count = Seat_Srv_FetchValidByRoomID(seat_head, schedule.studio_id);

    // 3. 以座位信息链表头指针和演出计划ID作为参数，调用持久化层存储演出票函数
    rtn = Ticket_Perst_Insert((ticket_list_t)seat_head, schedule_id);

    // 4. 返回count
    printf("业务逻辑层：生成完成，返回数量=%d\n", count);
    return count;
}

int Ticket_Srv_FetchAll(ticket_list_t list)
{
    return Ticket_Perst_SelAll(list);
}

int Ticket_Srv_FetchByID(int id, ticket_t* buf)
{
    return Ticket_Perst_SelByID(id, buf);
}

int Ticket_Srv_Modify(const ticket_t *data)
{
    return Ticket_Perst_Update(data);
}