//
// Created by shizihao on 2026/3/20.
//

#ifndef TTMS_STASALES_SRV_H
#define TTMS_STASALES_SRV_H
#include "../Common/common.h"

#endif // TTMS_STASALES_SRV_H
#ifndef SALE_H_
#define SALE_H_

#include "../Common/common.h"

// 交易类型
typedef enum
{
    SALE_SELL = 1,  // 买票
    SALE_RETURN = 2 // 退票
} sale_type_t;

// 订单信息实体
typedef struct
{
    int id;                // 订单ID
    int user_id;           // 售票员ID
    int ticket_id;         // 票ID
    user_date_t sale_date; // 销售日期（使用common.h中的user_date_t）
    int value;             // 金额
    sale_type_t type;      // 交易类型
} sale_t;

// 双向链表节点
typedef struct sale_node
{
    sale_t data;
    struct sale_node *next;
    struct sale_node *prev;
} sale_node_t, *sale_list_t;

int StaSales_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate);

#endif /* SALE_H_ */