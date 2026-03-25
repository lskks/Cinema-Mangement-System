/*
 * Copyright(C), 2007-2008, XUPT Univ.
 * 用例编号：TTMS_UC_02
 * File name: Seat.c
 * Description : 设置座位用例业务逻辑层
 * Author:   XUPT
 * Version:  v.1
 * Date: 	2015年4月22日
 */

#include "../Service/Seat.h"
#include "../Common/List.h"
#include "../Persistence/Seat_Persist.h"
#include <stdio.h>
#include <stdlib.h>

/*
功能：单个添加一个座位数据。
参数说明：data为seat_t类型指针，表示要添加的座位数据节点。
返回值：整型，表示是否成功添加座位的标志。
*/
int Seat_Srv_Add(const seat_t *data)
{
    // 参数校验
    if (data == NULL)
    {
        perror("data is null\n");
        return -1;
    }
    if (data->roomID <= 0 || data->row <= 0 || data->column <= 0)
    {
        return -1;
    }
    if (data->status != SEAT_NONE && data->status != SEAT_GOOD && data->status != SEAT_BROKEN)
    {
        return -1;
    }
    return Seat_Perst_Insert((seat_t *)data);
}

/*
功能：批量添加座位数据。
参数说明：list为seat_list_t类型指针，表示要添加的多个座位数据组成的链表。
返回值：整型，表示是否成功添加多个座位的标志。
*/
int Seat_Srv_AddBatch(seat_list_t list)
{
    // 参数校验
    if (list == NULL)
    {
        perror("链表为空\n");
        return -1;
    }
    seat_node_t *p;
    List_ForEach(list, p)
    {
        if (p->data.roomID <= 0 || p->data.row <= 0 || p->data.column <= 0)
        {
            return -1;
        }
        if (p->data.status != SEAT_NONE && p->data.status != SEAT_GOOD &&
            p->data.status != SEAT_BROKEN)
        {
            return -1;
        }
    }

    int loop = Seat_Perst_InsertBatch(list);
    return (loop > 0) ? 1 : 0;
}

/*
功能：单个修改一个座位数据。
参数说明：data为seat_t类型指针，表示要修改的座位数据节点。
返回值：整型，表示是否成功修改座位的标志。
*/
int Seat_Srv_Modify(const seat_t *data)
{
    // 参数校验
    if (data == NULL)
    {
        printf("list head is NULL\n");
        return -1;
    }

    if (data->id <= 0 || data->roomID <= 0 || data->row <= 0 || data->column <= 0)
    {
        printf("seat status is error\n");
        return -1;
    }
    if (data->status != SEAT_NONE && data->status != SEAT_GOOD && data->status != SEAT_BROKEN)
    {
        printf("seat status is error\n");
        return -1;
    }

    return Seat_Perst_Update((seat_t *)data);
}

/*
功能：根据座位ID删除一个座位。
参数说明：ID为整型，表示要删除的座位数据节点。
返回值：整型，表示是否成功删除座位的标志。
*/
int Seat_Srv_DeleteByID(int ID)
{
    // 参数校验

    if (ID <= 0)
    {
        printf("座位ID不合法\n");
        return -1;
    }

    return Seat_Perst_DeleteByID(ID);
}

/*
功能：根据座位ID获取座位数据。
参数说明：第一个参数ID为整型，表示座位ID；第二个参数buf为seat_t指针，指向获取到的座位数据节点。
返回值：整型，表示是否成功获取座位的标志。
*/
int Seat_Srv_FetchByID(int ID, seat_t *buf)
{
    // 参数校验

    if (ID <= 0)
    {
        printf("seat id is invalid\n");
        return -1;
    }
    if (buf == NULL)
    {
        printf("buf is null\n");
        return -1;
    }

    return Seat_Perst_SelectByID(ID, buf);
}

/*
功能：根据演出厅ID删除所有座位。
参数说明：roomID为整型，表示要删除所有座位的演出厅ID。
返回值：整型，表示是否成功删除演出厅所有座位的标志。
*/
/*inline*/ int Seat_Srv_DeleteAllByRoomID(int roomID)
{
    // 参数校验

    // 1. 合法性校验
    if (roomID <= 0)
    {
        printf("演出厅ID不合法\n");
        return -1;
    }

    return Seat_Perst_DeleteAllByRoomID(roomID);
}

/*
功能：根据演出厅ID获取演出厅所有座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返回值：整型，表示获取到的座位数量。
*/
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID)
{
    // 参数校验
    if (list == NULL)
    {
        printf("list is NULL\n");
        return -1;
    }
    if (roomID <= 0)
    {
        printf("Studio id is invalid\n");
        return -1;
    }

    return Seat_Perst_SelectByRoomID(list, roomID);
}

int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
    seat_node_t *pos;
    if (list == NULL)
    {
        printf("list is null\n");
        return -1;
    }
    if (roomID <= 0)
    {
        printf("id is less than 0\n");
        return -1;
    }

    int seatCount = Seat_Perst_SelectByRoomID(list, roomID);
    if (seatCount <= 0)
    {
        return seatCount;
    }

    List_ForEach(list, pos)
    {
        if (pos->data.status != SEAT_GOOD)
            seatCount--;
    }
    Seat_Srv_SortSeatList(list);
    return seatCount;

}

/*
功能：根据给定演出厅行、列数初始化演出厅所有座位数据，并将每个座位节点按行列排序加入链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数roomID为整型，表示演出厅ID，第三个参数rowsCount为整型，表示座位行数，第四个参数colsCount为整型，表示座位列数。
返回值：整型，表示成功初始化的座位数量。
*/
int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount, int colsCount)
{
    // 参数校验
    if (list == NULL)
    {
        printf("list head pointer is NULL\n");
        return -1;
    }
    if (roomID <= 0)
    {
        printf("Studio id is invalid\n");
        return -1;
    }
    if (rowsCount <= 0 || colsCount <= 0)
    {
        printf("行列数不合法\n");
        return -1;
    }

    seat_node_t *new_node;
    int row, col;

    for (row = 1; row <= rowsCount; row++)
    {
        for (col = 1; col <= colsCount; col++)
        {
            new_node = (seat_node_t *)malloc(sizeof(seat_node_t));
            if (new_node == NULL)
            {
                perror("allocate memory failed");
                return -1;
            }

            new_node->data.id = 0;
            new_node->data.roomID = roomID;
            new_node->data.row = row;
            new_node->data.column = col;
            new_node->data.status = SEAT_GOOD;

            List_AddTail(list, new_node);
        }
    }

    return rowsCount * colsCount;
}

/*
功能：对座位链表list按座位行号、列号进行排序。
参数说明：list为seat_list_t类型，表示待排序的座位链表头指针。
返回值：无。
*/
void Seat_Srv_SortSeatList(seat_list_t list)
{
    // 参数校验
    if (list == NULL || list->next == NULL)
    {
        fprintf(stderr, "list is NULL");
        return;
    }

    seat_node_t *p, *q;
    seat_t tmp;
    int change;

    List_ForEach(list, p)
    {
        change = 0;
        List_ForEach(list, q)
        {
            if (q->data.row > q->next->data.row)
            {
                tmp = q->data;
                q->data = q->next->data;
                q->next->data = tmp;
                change = 1;
            }
            else if (q->data.row == q->next->data.row && q->data.column > q->next->data.column)
            {
                tmp = q->data;
                q->data = q->next->data;
                q->next->data = tmp;
                change = 1;
            }
        }
        if (!change)
        {
            break;
        }
    }
}

/*
功能：将一个座位节点插入到已排序的座位链表中。
参数说明：第一个参数list为seat_list_t类型，表示已排序的座位链表头指针，第二个参数node为seat_node_t指针，表示要插入的座位数据节点。
返回值：无。
*/
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node)
{
    seat_node_t *p;
    int inserted = 0;
    if (list == NULL || node == NULL)
    {
        return;
    }

    List_ForEach(list, p)
    {
        if (p->data.row > node->data.row)
        {
            List_InsertBefore(p, node);
            inserted = 1;
            break;
        }
        else if (p->data.row == node->data.row && p->data.column > node->data.column)
        {
            List_InsertBefore(p, node);
            inserted = 1;
            break;
        }
        if (p->next == list)
        {
            List_AddTail(list, node);
            inserted = 1;
            break;
        }
    }
    if (!inserted)
    {
        List_AddTail(list, node);
    }
}

/*
功能：根据座位行号、列号获取座位数据。
参数说明：第一个参数list为seat_list_t类型，表示座位链表头指针，
         第二个参数row为整型，表示待获取座位的行号，第三个参数column为整型，表示待获取座位的列号。
返回值：为seat_node_t指针，表示获取到的座位数据。
*/
seat_node_t *Seat_Srv_FindByRowCol(seat_list_t list, int row, int column)
{
    seat_node_t *p;
    if (list == NULL)
    {
        return NULL;
    }
    List_ForEach(list, p)
    {
        if (p->data.row == row && p->data.column == column)
            return p;
    }
    return NULL;
}

/*
功能：根据座位ID在链表中获取座位数据。
参数说明：第一个参数list为seat_list_t类型，指向座位链表，第二个参数ID为整型，表示座位ID。
返回值：seat_node_t类型，表示获取到的座位数据。
*/
seat_node_t *Seat_Srv_FindByID(seat_list_t list, int rowID)
{
    seat_node_t *p;
    if (list == NULL)
    {
        return NULL;
    }
    List_ForEach(list, p)
    {
        if (p->data.id == rowID)
            return p;
    }
    return NULL;
}
