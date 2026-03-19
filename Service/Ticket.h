#ifndef TICKET_H_
#define TICKET_H_
typedef enum {
    TICKET_AVL = 0,    // 待售
    TICKET_SOLD = 1,   // 已售
    TICKET_RESV = 9    // 预留
} ticket_status_t;
typedef struct {
    int id;
    int schedule_id;
    int seat_id;
    int price;
    ticket_status_t status;
} ticket_t;

// 双向链表
typedef struct ticket_node {
    ticket_t data;
    struct ticket_node *next;
    struct ticket_node *prev;
} ticket_node_t, *ticket_list_t;

// TTMS_SCU_Ticket_Srv_GenBatch - 批量生成演出票
int Ticket_Srv_GenBatch(int schedule_id);

// TTMS_SCU_Ticket_Srv_DeleteBatch - 批量删除演出票
int Ticket_Srv_DeleteBatch(int schedule_id);

#endif /* TICKET_H_ */