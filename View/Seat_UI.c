

/*
 * Copyright(C), 2007-2008, XUPT Univ.
 * 用例编号：TTMS_UC_02
 * File name: Seat_UI.c
 * Description : 设置座位用例界面层
 * Author:   XUPT
 * Version:  v.1
 * Date: 	2015年4月22日
 */

#include "Seat_UI.h"
#include "../Service/Seat.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Common/common.h"
#include <stdio.h>

/*
表识符：TTMS_SCU_Seat_UI_S2C
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
inline char Seat_UI_Status2Char(seat_status_t status)
{
    switch (status)
    {
    case SEAT_GOOD:
        return '#';
    case SEAT_BROKEN:
        return '~';
    case SEAT_NONE:
        return ' ';
    }
}

/*
标识符：TTMS_SCU_Seat_UI_C2S
函数功能：根据输入符号获取座位状态。
参数说明：statusChar为字符型，表示设置座位的输入符号。
返 回 值：seat_status_t类型，表示座位的状态。
*/
inline seat_status_t Seat_UI_Char2Status(char statusChar)
{
    switch (statusChar)
    {
    case '#':
        return SEAT_GOOD;
    case '~':
        return SEAT_BROKEN;
    case ' ':
        return SEAT_NONE;
    }
	return SEAT_NONE;
}


void show_info(studio_t studio)
{
    printf("------------------------------------------------------------------\n");
    printf("Studio ID: %d\n", studio.id);
    printf("Studio Name: %s\n", studio.name);
    printf("Row Count: %d\n", studio.rowsCount);
    printf("Column Count: %d\n", studio.colsCount);
    printf("Seats Count: %d\n", studio.seatsCount);
    printf("------------------------------------------------------------------\n");
}

void Seat_UI_MgtEntry(int roomID)
{
    studio_t studio;
    char choice;
    seat_list_t list;
    int row, col;
    if (!Studio_Srv_FetchByID(roomID, &studio))
    {
        printf("Studio does not exist.\n");
        printf("Returning.\n");
        return;
    }
    else
    {
        List_Init(list, seat_node_t);
        int seatCount = Seat_Srv_FetchByRoomID(list, roomID);
        if (seatCount == 0)
        {
            printf("No seats found. Initializing seats...\n");
            Seat_Srv_RoomInit(list, roomID, studio.rowsCount, studio.colsCount);
            if (Seat_Srv_AddBatch(list))
            {
                studio.seatsCount = studio.rowsCount * studio.colsCount;
                Studio_Srv_Modify(&studio);
                printf("Seat initialization succeeded! Total initialized: %d.\n", studio.seatsCount);
            }
            else
            {
                printf("Seat initialization failed!\n");
            }
        }
        else
        {
            printf("Seats loaded successfully! Total seats: %d.\n", seatCount);
        }
        system(CLEAR);
        printf("==================================================================\n");
        printf("************************ Seat Management ************************\n");
        show_info(studio);
        printf("[A]dd Seat | [U]pdate Seat | [D]elete Seat | [R]eturn\n");
        printf("==================================================================\n");
        printf("Your Choice:");
        scanf(" %c", &choice);
        clear_input_buffer();
        switch (choice)
        {
        case 'A':
        case 'a':
            system(CLEAR);
            show_info(studio);
            
            printf("Input row to add (1-%d): ", studio.rowsCount);
            scanf("%d", &row);
            printf("Input column to add (1-%d):", studio.colsCount);
            scanf("%d", &col);

            clear_input_buffer();
            if (row >= 1 && row <= studio.rowsCount && col >= 1 && col <= studio.colsCount)
            {
                if (Seat_UI_Add(list, roomID, row, col))
                {
                    studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
                    Studio_Srv_Modify(&studio);
                }
            }
            else
            {
                printf("Row or column out of range!\n");
                printf("Please retry.\n");
            }
            List_Destroy(list, seat_node_t);
            List_Init(list, seat_node_t);
            Seat_Srv_FetchByRoomID(list, roomID);
            break;
        case 'U':
        case 'u':
            system(CLEAR);
            show_info(studio);
            clear_input_buffer();
            printf("Input row to update (1-%d):", studio.rowsCount);
            scanf("%d", &row);
            printf("Input column to update (1-%d):", studio.colsCount);
            scanf("%d", &col);
            clear_input_buffer();
            if (row >= 1 && row <= studio.rowsCount && col >= 1 && col <= studio.colsCount)
            {
                if (Seat_UI_Modify(list, row, col))
                {
                    // 更新演出厅座位数统计
                    studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
                    Studio_Srv_Modify(&studio);
                }
            }
            else
            {
                printf("Row or column out of range!\n");
                printf("Please retry.\n");
            }
            List_Destroy(list, seat_node_t);
            List_Init(list, seat_node_t);
            Seat_Srv_FetchByRoomID(list, roomID);
            break;
        case 'D':
        case 'd':
            system(CLEAR);
            show_info(studio);
            printf("Input row to delete (1-%d):", studio.rowsCount);
            scanf("%d", &row);
            clear_input_buffer();
            printf("Input column to delete (1-%d):", studio.colsCount);
            scanf("%d", &col);
            clear_input_buffer();
            if (row >= 1 && row <= studio.rowsCount && col >= 1 && col <= studio.colsCount)
            {
                if (Seat_UI_Delete(list, row, col))
                {
                    // 更新演出厅座位数统计
                    studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
                    Studio_Srv_Modify(&studio);
                }
            }
            else
            {
                printf("Row or column out of range!\n");
                printf("Please retry.\n");
            }
            List_Destroy(list, seat_node_t);
            List_Init(list, seat_node_t);
            Seat_Srv_FetchByRoomID(list, roomID);
            break;
        case 'R':
        case 'r':
            break;
        default:
            printf("Invalid input, please retry.\n");
            break;
        }
    }
}

/*
识符：TTMS_SCU_Seat_UI_Add
函数功能：用于添加一个新的座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，
         第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column)
{
    if (list == NULL)
    {
        printf("Error: seat list is null.\n");
        return 0;
    }
    if (roomID <= 0 || row <= 0 || column <= 0)
    {
        printf("Error: invalid studio ID, row, or column.\n");
        return 0;
    }   
    seat_list_t current = list;
    while (current != NULL)
    {
        if (current->data.roomID == roomID && current->data.row == row &&
            current->data.column == column)
        {
            printf("Error: seat already exists (Studio %d - Row %d, Col %d).\n", roomID, row, column);
            return 0;
        }
        current = current->next;
    }
    seat_t newSeat;
    newSeat.roomID = roomID;
    newSeat.row = row;
    newSeat.column = column;
    newSeat.status = SEAT_GOOD;
    int result = Seat_Srv_Add(&newSeat);
    if (result == 1)
    {
        printf("Success: seat added (Studio %d - Row %d, Col %d).\n", roomID, row, column);
        return 1;
    }
    else
    {
        printf("Error: failed to add seat.\n");
        return 0;
    }
}

/*
标识符：TTMS_SCU_Seat_UI_Modify
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column)
{
    seat_node_t *p = list;
    seat_t seat;
    int found = 0;
    char statusChar;
    while (p != NULL)
    {
        if (p->data.row == row && p->data.column == column)
        {
            found = 1;
            break;
        }
        p = p->next;
    }
    if (!found)
    {
        printf("Error: seat at row %d, col %d not found.\n", row, column);
        return -1;
    }
    printf("========== Seat Update ==========\n");
    printf("Current seat information:\n");
    printf("  Seat ID: %d\n", p->data.id);
    printf("  Studio ID: %d\n", p->data.roomID);
    printf("  Position: row %d, col %d\n", p->data.row, p->data.column);
    printf("  Current status: ");
    switch (p->data.status)
    {
    case SEAT_NONE:
        printf("None\n");
        break;
    case SEAT_GOOD:
        printf("Good\n");
        break;
    case SEAT_BROKEN:
        printf("Broken\n");
        break;
    default:
        printf("Unknown\n");
    }
    printf("\nInput new seat status:\n");
    printf("  # - Good seat\n");
    printf("  ~ - Broken seat\n");
    printf("    - No seat (space)\n");
    printf("Choose: ");
    scanf(" %c", &statusChar);
    clear_input_buffer();
    int new_status = Seat_UI_Char2Status(statusChar);
    if (new_status != SEAT_NONE && new_status != SEAT_GOOD && new_status != SEAT_BROKEN)
    {
        printf("Error: invalid seat status character!\n");
        return -1;
    }
    if (new_status == p->data.status)
    {
        printf("Notice: seat status unchanged.\n");
        return 1;
    }
    seat.id = p->data.id;
    seat.roomID = p->data.roomID;
    seat.row = p->data.row;
    seat.column = p->data.column;
    seat.status = new_status;
    int result = Seat_Srv_Modify(&seat);
    if (result == 1)
    {
        printf("Success: seat updated.\n");
        return 1;
    }
    else
    {
        printf("Failed: seat update failed.\n");
        return -1;
    }
}
/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column)
{
    seat_node_t *pos;
    seat_node_t *prev = NULL;
    int found = 0;
    char confirm;
    List_ForEach(list, pos)
    {
        if (pos->data.row == row && pos->data.column == column)
        {
            found = 1;
            break;
        }
        prev = pos;
        pos = pos->next;
    }
    if (!found)
    {
        printf("Error: seat at row %d, col %d not found.\n", row, column);
        return -1;
    }
    printf("========== Seat Deletion ==========\n");
    printf("Seat to delete:\n");
    printf("  Seat ID: %d\n", pos->data.id);
    printf("  Studio ID: %d\n", pos->data.roomID);
    printf("  Position: row %d, col %d\n", pos->data.row, pos->data.column);
    printf("  Current status: ");
    switch (pos->data.status)
    {
    case SEAT_NONE:
        printf("None\n");
        break;
    case SEAT_GOOD:
        printf("Good\n");
        break;
    case SEAT_BROKEN:
        printf("Broken\n");
        break;
    default:
        printf("Unknown\n");
    }
    printf("\nWarning: delete is irreversible!\n");
    printf("Confirm delete? (y/n): ");
    scanf(" %c", &confirm);
    clear_input_buffer();

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Operation cancelled.\n");
        return 0;
    }
    int result = Seat_Srv_DeleteByID(pos->data.id);
    if (result == 1)
    {
        printf("Success: seat deleted.\n");
        return 1;
    }
    else
    {
        printf("Failed: seat deletion failed.\n");
        return -1;
    }
}
