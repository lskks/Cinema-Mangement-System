

#ifndef SALESANALYSIS_H_
#define SALESANALYSIS_H_

#include "../Common/common.h"

// 剧目票房统计数据结构
typedef struct {
    int play_id;                // 剧目ID
    char play_name[31];         // 剧目名称
    char area[11];              // 剧目区域
    int sold_count;             // 上座数量（售票张数）
    float total_sale;           // 剧目票房（总收入）
    user_date_t start_date;     // 上映日期
    user_date_t end_date;       // 下映日期
    int status;                 // 状态：0-未上映，1-已上映，2-已下映
} salesanalysis_t;

// 双向链表
typedef struct salesanalysis_node {
    salesanalysis_t data;
    struct salesanalysis_node *next, *prev;
} salesanalysis_node_t, *salesanalysis_list_t;

// 标识符：TTMS_SCU_SalesAnalysis_Srv_StaticSale
// 功能：获取剧目票房信息，构建统计票房链表
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list);

// 标识符：TTMS_SCU_SalesAnalysis_Srv_SortBySale
// 功能：对剧目票房链表以票房为关键字降序排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);

#endif /* SALESANALYSIS_H_ */