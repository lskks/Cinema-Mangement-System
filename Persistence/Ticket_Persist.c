#include "Ticket_Persist.h"
#include "Schedule_Persist.h"
#include "Play_Persist.h"
#include "EntityKey_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const char TICKET_DATA_FILE[] = "Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat";

int Ticket_Perst_Insert(ticket_list_t list, int schedule_id) {
    FILE *fp;
    schedule_t sch;
    play_t play;
    seat_node_t *pos;
    ticket_t data;
    int count = 0;
    int rtn = 0;
    int i;
    long *new_ids;

    printf("持久化层：开始存储演出票\n");

    // 1. 以追加方式打开票数据文件
    fp = fopen(TICKET_DATA_FILE, "ab");
    if (fp == NULL) {
        printf("持久化层：无法打开票数据文件 %s\n", TICKET_DATA_FILE);
        return -1;
    }

    // 2. 调用Schedule_Perst_SelectByID函数获取演出计划信息
    if (Schedule_Srv_FetchByID(schedule_id, &sch) != 1) {
        printf("持久化层：获取演出计划失败\n");
        fclose(fp);
        return -1;
    }

    // 3. 调用Play_Perst_SelectByID函数获取剧目信息
    if (Play_Srv_FetchByID(sch.play_id, &play) != 1) {
        printf("持久化层：获取剧目信息失败\n");
        fclose(fp);
        return -1;
    }

    // 4. 统计座位信息链表的长度
    pos = (seat_node_t*)list;
    if (pos != NULL) {
        pos = pos->next;
        while (pos != (seat_node_t*)list) {
            count++;
            pos = pos->next;
        }
    }

    // 获取主键
    new_ids = (long*)malloc(count * sizeof(long));
    if (new_ids == NULL) {
        fclose(fp);
        return -1;
    }

    for (i = 0; i < count; i++) {
        new_ids[i] = EntKey_Perst_GetNewKeys("Ticket", 1);
    }

    // 5. 遍历链表，构造票信息并写入文件
    pos = (seat_node_t*)list;
    if (pos != NULL) {
        pos = pos->next;
        i = 0;
        while (pos != (seat_node_t*)list) {
            data.id = new_ids[i];
            data.schedule_id = schedule_id;
            data.seat_id = pos->data.id;
            data.price = play.price;
            data.status = TICKET_AVL;

            if (fwrite(&data, sizeof(ticket_t), 1, fp) == 1) {
                rtn = 1;
            } else {
                rtn = 0;
                break;
            }

            pos = pos->next;
            i++;
        }
    }

    printf("持久化层：成功写入 %d 张票\n", i);

    // 6. 关闭文件
    fclose(fp);
    free(new_ids);

    return rtn;
}


int Ticket_Perst_Rem(int schedule_id) {
    FILE *fp_old, *fp_new;
    ticket_t buf;
    int found = 0;

    printf("持久化层：开始删除演出计划ID=%d的票\n", schedule_id);

    // a) 将票据数据文件改名
    if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) != 0) {
        printf("持久化层：文件改名失败\n");
        return -1;
    }

    // b) 以只读方式打开临时文件，只写方式打开新文件
    fp_old = fopen(TICKET_DATA_TEMP_FILE, "rb");
    fp_new = fopen(TICKET_DATA_FILE, "wb");

    if (fp_old == NULL || fp_new == NULL) {
        printf("持久化层：文件打开失败\n");
        if (fp_old) fclose(fp_old);
        if (fp_new) fclose(fp_new);
        rename(TICKET_DATA_TEMP_FILE, TICKET_DATA_FILE);
        return -1;
    }

    // c) 计数器清0，循环读取
    found = 0;
    while (!feof(fp_old)) {
        // d) 从文件读出一条记录
        if (fread(&buf, sizeof(ticket_t), 1, fp_old) != 1) {
            break;
        }

        // e) 判断是否等于参数schedule_id
        if (buf.schedule_id == schedule_id) {
            found++; // 相等则计数加1，不写入新文件
        } else {
            // 不相等则写入新文件
            fwrite(&buf, sizeof(ticket_t), 1, fp_new);
        }
    }

    // f) 关闭文件，恢复原名，返回结果
    fclose(fp_old);
    fclose(fp_new);

    remove(TICKET_DATA_TEMP_FILE);

    printf("持久化层：共删除 %d 张票\n", found);
    return found;
}

int Ticket_Perst_SelAll(ticket_list_t list)
{
    ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

int Ticket_Perst_SelByID(int id, ticket_t* buf)
{
    int found = 0;
    ticket_t data;
    FILE* fp = fopen(TICKET_DATA_FILE, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "open %s file faild\n", TICKET_DATA_FILE);
        return 0;
    }

    while (fread(&data, sizeof(ticket_t), 1, fp) == 1)
    {
        if (data.id == id)
        {
            memcpy(buf, &data, sizeof(ticket_t));
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}