#include "SaleAnalysis.h"
#include "../Common/List.h"
#include "../Service/Play.h"
#include <assert.h>
#include <string.h>

int SaleAnalysis_Srv_StaticSale(sale_analysis_list_t list)
{
    assert(list != NULL);

    play_list_t play_list;
    play_node_t *pos;
    sale_analysis_node_t *newNode;

    int sold = 0, cnt = 0;

    List_Free(list, sale_analysis_node_t);

    List_Init(play_list, play_node_t);

    Play_Srv_FetchAll(play_list);
    List_ForEach(play_list, pos)
    {
        newNode = (sale_analysis_node_t *)malloc(sizeof(sale_analysis_node_t));
        newNode->data.playId = pos->data.id;
        strcpy(newNode->data.name, pos->data.name);
        strcpy(newNode->data.area, pos->data.area);
        newNode->data.duration = pos->data.duration;
        newNode->data.price = pos->data.price;
        newNode->data.start_date = pos->data.start_date;
        newNode->data.end_date = pos->data.end_date;
        // newNode->data.sales = Schedule_StatSaleByPlayID(pos->data.id);
        newNode->data.total_ticket = sold;
        List_AddTail(list, newNode);
        cnt++;
    }
    List_Destroy(list, sale_analysis_node_t);
    return cnt;
}

void SaleAnalysis_Srv_SortBySale(sale_analysis_list_t list)
{
    // 参数校验
    if (list == NULL || list->next == NULL)
        return;

    sale_analysis_node_t *p, *q;
    sale_analysis_t tmp;
    int change;

    for (p = list; p->next != NULL; p = p->next)
    {
        change = 0;
        for (q = list; q->next != NULL; q = q->next)
        {
            if (q->data.total_ticket > q->next->data.total_ticket)
            {
                tmp = q->data;
                q->data = q->next->data;
                tmp = q->data;
                q->data = q->next->data;
                q->next->data = tmp;
                change = 1;
            }
            else if (q->data.total_ticket == q->next->data.total_ticket && q->data.total_ticket > q->next->data.total_ticket)
            {
                tmp = q->data;
                q->data = q->next->data;
                q->next->data = tmp;
                change = 1;
            }
        }
        if (!change)
            break;
        }
}