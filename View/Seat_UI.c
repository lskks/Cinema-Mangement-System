












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

#include "../Service/Studio.h"
#include "../Common/List.h"
#include <stdio.h>

/*
表识符：TTMS_SCU_Seat_UI_S2C
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
inline char Seat_UI_Status2Char(seat_status_t status) {
        switch (status) {
        	case SEAT_GOOD:
        		return '#';
        	case  SEAT_BROKEN:
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
inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	switch (statusChar) {
		case '#':
			return SEAT_GOOD;
			case '~':
			return SEAT_BROKEN;
			case ' ':
			return SEAT_NONE;
	}
}

/*
标识符：TTMS_SCU_Seat_UI_MgtEnt
函数功能：界面层管理座位的入口函数，显示当前的座位数据，并提供座位数据添加、修改、删除功能操作的入口。
参数说明：roomID为整型，是需要设置座位的演出厅ID。
返 回 值：无。
*/
void Seat_UI_MgtEntry(int roomID) {
	studio_t studio;
	char choice;
	seat_list_t list;
	int row,col;
	if (!Studio_Srv_FetchByID(roomID,&studio)){
		printf("演出厅不存在\n");
		printf("返回");
		return;
	}
	else {
		List_Init(list, seat_node_t);
		int seatCount=Seat_Srv_FetchByRoomID(list,roomID);
		if (seatCount==0) {
            printf("座位不存在，正在进行座位初始化");
			Seat_Srv_RoomInit(list,roomID,studio.rowsCount,studio.colsCount);
			if (Seat_Srv_AddBatch(list)) {
				studio.seatsCount = studio.rowsCount * studio.colsCount;
				Studio_Srv_Modify(&studio);
				printf("座位初始化成功！共初始化 %d 个座位\n", studio.seatsCount);
			}
			else {
				printf("座位初始化失败！\n");
			}
		}
		else {

		}

	printf("[A]添加座位 | [U]修改座位 | [D]删除座位 | [R]返回\n");
	printf("请选择操作:");
	scanf("%c", &choice);
	switch (choice) {
		case 'A':
		case 'a':
			printf("请输入要添加的行号(1-%d):\n请输入要添加的列号(1-%d):",studio.rowsCount,studio.colsCount);
			scanf("%d", &row);
			scanf("%d", &col);
			if (row>=1&&row<=studio.rowsCount&&col>=1&&col<=studio.colsCount) {
				if (Seat_UI_Add(list, roomID, row, col)) {
					studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
					Studio_Srv_Modify(&studio);
				}
			} else {
				printf("输入的行列号超出范围！\n");
				printf("重新输入\n");
			}
			List_Destroy(list, seat_node_t);
			List_Init(list, seat_node_t);
			Seat_Srv_FetchByRoomID(list, roomID);
			break;
			case 'U':
		    case 'u':
			printf("请输入要修改座位的行号(1-%d):", studio.rowsCount);
			scanf("%d", &row);
			printf("请输入要修改座位的列号(1-%d):", studio.colsCount);
			scanf("%d", &col);
			if (row >= 1 && row <= studio.rowsCount &&
				col >= 1 && col <= studio.colsCount) {
				if (Seat_UI_Modify(list, row, col)) {
					// 更新演出厅座位数统计
					studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
					Studio_Srv_Modify(&studio);
				}
				} else {
					printf("输入的行列号超出范围！\n");
					printf("重新输入\n");
				}
			List_Destroy(list, seat_node_t);
			List_Init(list, seat_node_t);
			Seat_Srv_FetchByRoomID(list, roomID);
			break;
			case 'D':
			case 'd':
			printf("请输入要删除座位的行号(1-%d):", studio.rowsCount);
			scanf("%d", &row);
			printf("请输入要删除座位的列号(1-%d):", studio.colsCount);
			scanf("%d", &col);
			if (row >= 1 && row <= studio.rowsCount &&
				col >= 1 && col <= studio.colsCount) {
				if (Seat_UI_Delete(list, row, col)) {
					// 更新演出厅座位数统计
					studio.seatsCount = Seat_Srv_FetchByRoomID(list, roomID);
					Studio_Srv_Modify(&studio);
				}
				} else {
					printf("输入的行列号超出范围！\n");
					printf("重新输入\n");
				}
			List_Destroy(list, seat_node_t);
			List_Init(list, seat_node_t);
			Seat_Srv_FetchByRoomID(list, roomID);
			break;
			case 'R':
			case 'r':
			break;
		default:
			printf("无效输入，请重新输入\n");
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
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ
    if (list==NULL) {
	    printf("错误:座位列表为空");
    	return 0;
    }
	if (roomID<=0||row<=0||column<=0) {
		printf("错误:演出厅ID、排号或列号\n");
		return 0;
	}
	seat_list_t current=list;
	while (current != NULL) {
		if (current->data.roomID == roomID &&
			current->data.row == row &&
			current->data.column == column) {
			printf("错误：该座位已存在（演出厅%d - %d排%d座）\n", roomID, row, column);
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
	if (result == 1) {
		printf("成功：座位已添加（演出厅%d - %d排%d座）\n", roomID, row, column);
		return 1;
	} else {
		printf("错误：添加座位失败\n");
		return 0;
	}
}



/*
标识符：TTMS_SCU_Seat_UI_Modify
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
    seat_node_t* p = list;
    seat_t seat;
    int found = 0;
    char statusChar;
    while (p != NULL) {
        if (p->data.row == row && p->data.column == column) {
            found = 1;
            break;
        }
        p = p->next;
    }
    if (!found) {
        printf("错误：未找到第%d行第%d列的座位！\n", row, column);
        return -1;
    }
    printf("========== 座位修改 ==========\n");
    printf("当前座位信息：\n");
    printf("  座位ID：%d\n", p->data.id);
    printf("  所属影厅ID：%d\n", p->data.roomID);
    printf("  位置：第%d行 第%d列\n", p->data.row, p->data.column);
    printf("  当前状态：");
    switch(p->data.status) {
        case SEAT_NONE:
            printf("无座位\n");
            break;
        case SEAT_GOOD:
            printf("完好\n");
            break;
        case SEAT_BROKEN:
            printf("损坏\n");
            break;
        default:
            printf("未知状态\n");
    }
    printf("\n请输入新的座位状态：\n");
    printf("  # - 完好座位\n");
    printf("  ~ - 损坏座位\n");
    printf("  | - 无座位\n");
    printf("请选择：");
    scanf(" %c", &statusChar);
    int new_status = Seat_UI_Char2Status(statusChar);
    if (new_status != SEAT_NONE &&
        new_status != SEAT_GOOD &&
        new_status != SEAT_BROKEN) {
        printf("错误：无效的座位状态字符！\n");
        return -1;
    }
    if (new_status == p->data.status) {
        printf("提示：座位状态未改变，无需修改。\n");
        return 1;
    }
    seat.id = p->data.id;
    seat.roomID = p->data.roomID;
    seat.row = p->data.row;
    seat.column = p->data.column;
    seat.status = new_status;
    int result = Seat_Srv_Modify(&seat);
    if (result == 1) {
        printf("成功：座位修改成功！\n");
        return 1;
    } else {
        printf("失败：座位修改失败！\n");
        return -1;
    }
}
/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
	seat_node_t* p = list;
	seat_node_t* prev = NULL;
	int found = 0;
	char confirm;
	while (p != NULL) {
		if (p->data.row == row && p->data.column == column) {
			found = 1;
			break;
		}
		prev = p;
		p = p->next;
	}
	if (!found) {
		printf("错误：未找到第%d行第%d列的座位！\n", row, column);
		return -1;
	}
	printf("========== 座位删除 ==========\n");
	printf("即将删除的座位信息：\n");
	printf("  座位ID：%d\n", p->data.id);
	printf("  所属影厅ID：%d\n", p->data.roomID);
	printf("  位置：第%d行 第%d列\n", p->data.row, p->data.column);
	printf("  当前状态：");
	switch(p->data.status) {
		case SEAT_NONE:
			printf("无座位\n");
			break;
		case SEAT_GOOD:
			printf("完好\n");
			break;
		case SEAT_BROKEN:
			printf("损坏\n");
			break;
		default:
			printf("未知状态\n");
	}
	printf("\n警告：删除操作不可恢复！\n");
	printf("是否确认删除？(y/n)：");
	scanf(" %c", &confirm);

	if (confirm != 'y' && confirm != 'Y') {
		printf("操作已取消。\n");
		return 0;
	}
	int result = Seat_Srv_DeleteAllByRoomID(p->data.id);
	if (result == 1) {
		printf("成功：座位删除成功！\n");
		return 1;
	} else {
		printf("失败：座位删除失败！\n");
		return -1;
	}
}




