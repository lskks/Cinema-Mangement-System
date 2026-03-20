#include "SalesAnalysis_UI.h"
#include "../Service/SalesAnalysis.h"
#include "../Common/List.h"
#include <stdio.h>

#define SALESANALYSIS_PAGE_SIZE 5


void SalesAnalysis_UI_MgtEntry(void) {
    int i;
    char choice;
    salesanalysis_list_t list;
    salesanalysis_node_t *pos;
    Pagination_t paging;

    // a) 初始化统计票房链表
    List_Init(list, salesanalysis_node_t);

    // b) 设置分页显示页面大小
    paging.offset = 0;
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;

    // c) 调用获取剧目票房信息函数，构建统计票房链表
    // d) 设置分页的总记录数
    paging.totalRecords = SalesAnalysis_Srv_StaticSale(list);

    // e) 调用剧目票房排行函数，对链表降序排序
    SalesAnalysis_Srv_SortBySale(list);

    // 定位到第一页
    Paging_Locate_FirstPage(list, paging);

    system(CLEAR);

    do {
        printf("\n==================================================================\n");
        printf("********************** Box Office Ranking **********************\n");
        printf("%-5s %-20s %-8s %-10s %-12s %-12s %-12s\n",
               "Rank", "Play Name", "Area", "Sold Count", "Box Office", "Start Date", "End Date");
        printf("------------------------------------------------------------------\n");

        // f) 分页显示剧目票房排行信息
        int rank = paging.offset + 1;
        Paging_ViewPage_ForEach(list, paging, salesanalysis_node_t, pos, i) {
            printf("%-5d %-20s %-8s %-10d %-12.2f %04d-%02d-%02d  %04d-%02d-%02d\n",
                   rank++,
                   pos->data.play_name,
                   pos->data.area,
                   pos->data.sold_count,
                   pos->data.total_sale,
                   pos->data.start_date.year,
                   pos->data.start_date.month,
                   pos->data.start_date.day,
                   pos->data.end_date.year,
                   pos->data.end_date.month,
                   pos->data.end_date.day);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        fflush(stdin);
        scanf("%c", &choice);
        fflush(stdin);
        
        // g) 接收用户输入的按键字符
        switch (choice) {
        case 'p':
        case 'P':
            // h) 显示上一页
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, -1, salesanalysis_node_t);
            }
            break;
            case 'n':
            case 'N':
            // i) 显示下一页
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, 1, salesanalysis_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');
    system(CLEAR);
    
    // j) 销毁统计票房链表
    List_Destroy(list, salesanalysis_node_t);
}