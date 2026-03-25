#include "../Service/Schedule.h"
#include "../Common/List.h"
#include "EntityKey_Persist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";

int Schedule_Perst_SelectAll(schedule_list_t list) {
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;

    assert(NULL != list);

    List_Free(list, schedule_node_t);

    // 判断文件是否存在
    if (access(SCHEDULE_DATA_FILE, 0) != 0) {
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
            if (!newNode) {
                printf("Warning, Memory OverFlow!!!\nCannot Load more data into memory!!!\n");
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

int Schedule_Perst_SelectByID(schedule_list_t list, int play_id) {
    schedule_node_t *newNode;
    schedule_t data;
    int recCount = 0;

    assert(NULL != list);

    List_Free(list, schedule_node_t);

    // 判断文件是否存在
    if (access(SCHEDULE_DATA_FILE, 0) != 0) {
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) {
        return 0;
    }

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            if (data.play_id == play_id) {
                newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
                if (!newNode) {
                    printf("Warning, Memory OverFlow!!!\nCannot Load more data into memory!!!\n");
                    break;
                }
                newNode->data = data;
                List_AddTail(list, newNode);
                recCount++;
            }
        }
    }

    fclose(fp);
    return recCount;
}

int Schedule_Perst_Insert(schedule_t *data)
{
    assert(NULL != data);

    long key = EntKey_Perst_GetNewKeys("schedule", 1);
    if (key <= 0)
    {
        fprintf(stderr, "Allocting key failed\n");
        return 0;
    }

    data->id = key;

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
    if (NULL == fp) {
        fprintf(stderr, "open file failed: %s\n", SCHEDULE_DATA_FILE);
        return 0;
    }

    int result = fwrite(data, sizeof(schedule_t), 1, fp);
    fclose(fp);
    return result;
}

int Schedule_Perst_Modify(schedule_t *data)
{
    assert(NULL != data);

    schedule_list_t list;
    schedule_node_t *pos;
    int found = 0;

    List_Init(list, schedule_node_t);
    Schedule_Perst_SelectAll(list);

    List_ForEach(list, pos) {
        if (pos->data.id == data->id) {
            pos->data = *data;
            found = 1;
            break;
        }
    }

    if (!found) {
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "wb");
    if (NULL == fp) {
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    List_ForEach(list, pos) {
        fwrite(&pos->data, sizeof(schedule_t), 1, fp);
    }

    fclose(fp);
    List_Destroy(list, schedule_node_t);
    return 1;
}

int Schedule_Perst_DeleteByID(int id) {
    schedule_list_t list;
    schedule_node_t *pos;
    int found = 0;

    List_Init(list, schedule_node_t);
    Schedule_Perst_SelectAll(list);

    List_ForEach(list, pos) {
        if (pos->data.id == id) {
            List_DelNode(pos);
            found = 1;
            break;
        }
    }

    if (!found) {
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "wb");
    if (NULL == fp) {
        List_Destroy(list, schedule_node_t);
        return 0;
    }

    List_ForEach(list, pos) {
        fwrite(&pos->data, sizeof(schedule_t), 1, fp);
    }

    fclose(fp);
    List_Destroy(list, schedule_node_t);
    return 1;
}

void Schedule_Perst_SelectByIDForSchedule(schedule_t *buf, int id)
{
    assert(NULL != buf);

    FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
    if (NULL == fp) {
        fprintf(stderr, "open file failed: %s\n", SCHEDULE_DATA_FILE);
        return;
    }

    schedule_t data;
    int found = 0;

    while (!feof(fp)) {
        if (fread(&data, sizeof(schedule_t), 1, fp)) {
            if (data.id == id) {
                *buf = data;
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
}