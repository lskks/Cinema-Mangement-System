#ifndef SALE_ANALYSIS_H_
#define SALE_ANALYSIS_H_

#include "Play.h"

typedef struct 
{
    int playId;
    char name[30];
    char area[30];
    int duration;
    long total_ticket;
    long sales;
    int price;
    ttms_date_t start_date;
    ttms_date_t end_date;
} sale_analysis_t;

typedef struct sale_analysis_node
{
    sale_analysis_t data;
    struct sale_analysis_node *next;
    struct sale_analysis_node *prev;
} sale_analysis_node_t, *sale_analysis_list_t;
 
int SaleAnalysis_Srv_StaticSale(sale_analysis_list_t list);

void SaleAnalysis_Srv_SortBySale(sale_analysis_list_t list);

#endif