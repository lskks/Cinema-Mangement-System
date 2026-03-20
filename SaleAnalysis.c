

#include "SalesAnalysis.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


extern int Play_Srv_FetchAll(void *list);
extern int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount);


int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list) {

    assert(NULL != list);


    void *playlist;
    void *pos;


    salesanalysis_node_t *newNode;


    List_Free(list, salesanalysis_node_t);


    List_Init(playlist, void);


    Play_Srv_FetchAll(playlist);


    List_ForEach(playlist, pos) {
        newNode = (salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));
        if (!newNode) {
            printf("Warning, Memory OverFlow!!!\n");
            break;
        }

        // 填充剧目基本信息
        newNode->data.play_id = pos->data.id;
        strcpy(newNode->data.play_name, pos->data.name);
        strcpy(newNode->data.area, pos->data.area);
        newNode->data.start_date = pos->data.start_date;
        newNode->data.end_date = pos->data.end_date;
        newNode->data.status = pos->data.status;

        // 调用Schedule_Srv_StatRevByPlay统计该剧目的票房和售票数量
        newNode->data.total_sale = Schedule_Srv_StatRevByPlay(pos->data.id, &(newNode->data.sold_count));

        List_AddTail(list, newNode);
    }
    */

    // i) 调用List_Destroy销毁链表playlist
    List_Destroy(playlist, void);

    // j) 返回list链表的结点个数
    int count = 0;
    salesanalysis_node_t *tmp = list->next;
    while (tmp != list) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

/*
标识符：TTMS_SCU_SalesAnalysis_Srv_SortBySale
函数功能：对salesanalysis_list_t类型链表以剧目票房为关键字降序排序
参数说明：list为salesanalysis_list_t类型，表示链表的头指针
返回值：无
*/
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list) {
    assert(NULL != list);

    if (List_IsEmpty(list)) {
        return;
    }

    // 使用选择排序法，以票房为关键字降序排序
    salesanalysis_node_t *i, *j;
    salesanalysis_t temp;

    for (i = list->next; i != list && i->next != list; i = i->next) {
        for (j = i->next; j != list; j = j->next) {
            if (i->data.total_sale < j->data.total_sale) {
                // 交换数据
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}