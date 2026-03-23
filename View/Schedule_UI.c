//
// Created by wjw on 2026/3/19.
//

#include "Schedule_UI.h"
#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Common/common.h"
#include "Ticket_UI.h"
#include <stdio.h>
#include <string.h>

static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_MgtEntry(int playID)
{
    int i, id;
    play_t data;
    Pagination_t paging;
    schedule_list_t list;
    schedule_node_t *pos;
    schedule_t buf;
    char choice;
    if (!Play_Srv_FetchByID(playID, &data))
    {
        fprintf(stderr, "Play does not exist.\n");
        return;
    }

    List_Init(list, schedule_node_t);

    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;
    paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);

    Paging_Locate_FirstPage(list, paging);

    do
    {
        printf("************************* Schedule Management *************************\n");
        printf("%4s  %-8s  %-8s  %-8s  %-6s  %-5s\n", "ID", "Play ID", "Studio ID", "Seat Count", "Dur", "Price");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
        {
            printf("%4d  %-8d  %-8d  %-8d  %-6d  %-5.2d\n", pos->data.id,
                   pos->data.play_id, pos->data.studio_id, pos->data.seat_count,
                   data.duration, data.price);
        }
        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]how Ticket | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice)
        {
        case 'a':
        case 'A':
            system(CLEAR);
            printf("Play ID: ");
            scanf("%d", &playID);
            clear_input_buffer();
            Schedule_UI_Add(playID);
            paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
            List_Paging(list, paging, schedule_node_t);
            break;
        case 'd':
        case 'D':
            system(CLEAR);
            printf("Enter the schedule ID to delete: ");
            scanf("%d", &id);
            clear_input_buffer();
            Schedule_UI_Delete(id);
            paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
            List_Paging(list, paging, schedule_node_t);
            break;
            case 'u':
            case 'U':
            system(CLEAR);
            printf("Enter the schedule ID to update: ");
            scanf("%d", &id);
            clear_input_buffer();
            Schedule_UI_Modify(id);
            paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
            List_Paging(list, paging, schedule_node_t);
            break;
            case 's':
            case 'S':
            system(CLEAR);
            printf("Enter the schedule ID to query: ");
            scanf("%d", &id);
            clear_input_buffer();
            if (Schedule_Srv_FetchByID(id, &buf) == 0)
            {
                fprintf(stderr, "Schedule not found.\n");
                break;
            }
            Ticket_UI_MgtEntry(id);
            break;
        case 'p':
        case 'P':
            system(CLEAR);
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
            }
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');

    system(CLEAR);
    List_Destroy(list, schedule_node_t);

}

int Schedule_UI_Add(int playID)
{
    schedule_t data;
    memset(&data, 0, sizeof(schedule_t));

    printf("=======================================================\n");
    printf("********************** Add New Schedule *******************\n");
    data.play_id = playID;
    printf("Studio ID: ");
    scanf("%d", &data.studio_id);
    printf("End Date (YYYY MM DD): ");
    scanf("%d %d %d", &data.date.year, &data.date.month, &data.date.day);
    printf("End Time (HH MM SS): ");
    scanf("%d %d %d", &data.time.hour, &data.time.minute, &data.time.second);
    printf("Price: ");
    scanf("%f", &data.price);
    printf("Status of Schedule (0-Not Started, 1-Started, 2-Ended): ");
    scanf("%d", &data.status);
    clear_input_buffer();
    printf("=======================================================\n");

    if (Schedule_Srv_Add(&data))
    {
        printf("Schedule added successfully! ID=%d\n", data.id);
        return 1;
    }

    printf("Failed to add schedule.\n");
    return 0;
}

int Schedule_UI_Modify(int playID)
{
    schedule_t data;
    if (!Schedule_Srv_FetchByID(playID, &data))
    {
        printf("Schedule does not exist.\n");
        return 0;
    }

    printf("=======================================================\n");
    printf("********************** Update Schedule ********************\n");
    printf("Schedule ID: ");
    scanf("%d", &data.id);
    printf("Play ID: ");
    scanf("%d", &data.play_id);
    printf("Studio ID: ");
    scanf("%d", &data.studio_id);
    printf("End Date (YYYY MM DD): ");
    scanf("%d %d %d", &data.date.year, &data.date.month, &data.date.day);
    printf("End Time (HH MM SS): ");
    scanf("%d %d %d", &data.time.hour, &data.time.minute, &data.time.second);
    printf("Price: ");
    scanf("%f", &data.price);
    printf("Status of Schedule (0-Not Started, 1-Started, 2-Ended): ");
    scanf("%d", &data.status);
    clear_input_buffer();
    printf("=======================================================\n");

    if (Schedule_Srv_Modify(&data))
    {
        printf("Schedule updated successfully.\n");
        return 1;
    }

    printf("Failed to update schedule.\n");
    return 0;
}

int Schedule_UI_Delete(int playID)
{
    char confirm;
    printf("Are you sure to delete schedule ID %d? (y/n): ", playID);
    scanf(" %c", &confirm);
    clear_input_buffer();

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Delete cancelled.\n");
        return 0;
    }

    if (Schedule_Srv_DeleteByID(playID))
    {
        printf("Schedule deleted successfully.\n");
        return 1;
    }

    printf("Failed to delete schedule (may not exist).\n");
    return 0;
}
