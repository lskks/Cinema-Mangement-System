//
// Created by shizihao on 2026/3/20.
//
#include "StaSales.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Persistence/StaSales_Persist.h"
#include "../Persistence/Sale_Persist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
int StaSales_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate)
{
    int amount = 0;
    sale_list_t saleList = NULL;
    sale_node_t *pSale;
    List_Init(saleList, sale_node_t);
    if (saleList == NULL)
    {
        printf("业务逻辑层:初始化链表失败!\n");
        return -1;
    }
    int recCount = Sale_Perst_SelByID(saleList, usrID);
    printf("业务逻辑层:载入用户ID=%d的销售记录%d条\n", usrID, recCount);
    if (recCount <= 0)
    {
        printf("业务逻辑层:用户ID=%d没有销售记录\n", usrID);
        List_Destroy(saleList, sale_node_t);
        return 0;
    }
    List_ForEach(saleList, pSale)
    {
        if (pSale->data.type == 1)
        {
            if (DateCmp(pSale->data.sale_date, stDate) >= 0 &&
                DateCmp(pSale->data.sale_date, endDate) <= 0)
            {
                amount += pSale->data.value;
                printf("业务逻辑层:累加记录-日期=%04d-%02d-%02d,金额=%d,当前总计=%d\n",
                       pSale->data.sale_date.year, pSale->data.sale_date.month,
                       pSale->data.sale_date.day, pSale->data.value, amount);
            }
        }
    }
    printf("业务逻辑层:用户ID=%d的销售额统计为%d\n", usrID, amount);
    List_Destroy(saleList, sale_node_t);
    return amount;
}