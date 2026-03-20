//
// Created by shizihao on 2026/3/20.
//
#include "StaSales_Persist.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const char SALE_DATA_FILE[] = "Sale.dat";
int Sale_Perst_SelByID(sale_list_t list, int usrID) {
    FILE *fp;
    sale_t data;
    sale_node_t *newNode;
    int recCount=0;
    assert(list!=NULL);
    List_Free(list,sale_node_t);
    fp = fopen(SALE_DATA_FILE,"rb");
    if (fp==NULL) {
        printf("持久化层:文件%s不存在或无法打开\n",SALE_DATA_FILE);
        return 0;
    }
    while(!feof(fp)) {
        if (fread(&data, sizeof(sale_t), 1, fp) == 1) {
            // 匹配用户ID
            if (data.user_id == usrID) {
                // 创建新节点
                newNode = (sale_node_t*)malloc(sizeof(sale_node_t));
                if (NULL == newNode) {
                    printf("警告：内存不足，无法载入更多数据！\n");
                    break;
                }
                newNode->data = data;
                List_AddTail(list,newNode);
                recCount++;
            }
        }
    }
    fclose(fp);
    printf("持久化层：为用户ID=%d载入%d条销售记录\n", usrID, recCount);
    return recCount;
}