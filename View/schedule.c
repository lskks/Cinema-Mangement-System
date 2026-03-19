//
// Created by wjw on 2026/3/19.
///*
*/

#include "Schedule.h"
#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include <stdio.h>
#include <string.h>

static const int SCHEDULE_PAGE_SIZE = 5;


void Schedule_UI_ListAll(void) {
    int i;
    char choice;

    schedule_list_t list_schedule;
    schedule_node_t *pos_schedule;

    play_list_t list_play;
    play_node_t *play_node;

    studio_list_t list_studio;
    studio_node_t *studio_node;

    Pagination_t paging;

    // 初始化链表
    List_Init(list_schedule, schedule_node_t);
    List_Init(list_play, play_node_t);
    List_Init(list_studio, studio_node_t);

    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;

    // 载入数据
    paging.totalRecords = Schedule_Srv_FetchAll(list_schedule);

    // 载入所有剧目和演出厅信息，用于显示名称
    Play_Srv_FetchAll(list_play);
    Studio_Srv_FetchAll(list_studio);

    Paging_Locate_FirstPage(list_schedule, paging);

    do {
        printf("\n==================================================================\n");
        printf("************************ Schedule List ************************\n");
        printf("%5s  %20s  %12s  %12s  %10s  %8s\n",
               "ID", "Play Name", "Studio Name", "Date", "Start Time", "Price");
        printf("------------------------------------------------------------------\n");

        // 显示数据
        Paging_ViewPage_ForEach(list_schedule, paging, schedule_node_t, pos_schedule, i) {
            // 获取剧目名称
            play_node = Play_Srv_FindByID(list_play, pos_schedule->data.play_id);
            char *play_name = (play_node) ? play_node->data.name : "Unknown";

            // 获取演出厅名称
            studio_node = Studio_Srv_FindByID(list_studio, pos_schedule->data.studio_id);
            char *studio_name = (studio_node) ? studio_node->data.name : "Unknown";

            printf("%5d  %20s  %12s  %04d-%02d-%02d  %02d:%02d     %8.2f\n",
                   pos_schedule->data.id,
                   play_name,
                   studio_name,
                   pos_schedule->data.date.year,
                   pos_schedule->data.date.month,
                   pos_schedule->data.date.day,
                   pos_schedule->data.time.hour,
                   pos_schedule->data.time.minute,
                   pos_schedule->data.price);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging),
               Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [Q]uery | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        fflush(stdin);
        scanf("%c", &choice);
        fflush(stdin);

        switch (choice) {
        case 'q':
        case 'Q':
            Schedule_UI_Query(NULL);
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(list_schedule, paging, -1, schedule_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(list_schedule, paging, 1, schedule_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');

    // 释放链表空间
    List_Destroy(list_schedule, schedule_node_t);
    List_Destroy(list_play, play_node_t);
    List_Destroy(list_studio, studio_node_t);
}

int Schedule_UI_Query(char *play_name) {
    static char name[31];
    play_list_t list_play;
    play_node_t *play_node;

    schedule_list_t list_schedule;
    schedule_node_t *schedule_node;

    studio_list_t list_studio;
    studio_node_t *studio_node;

    int play_count, schedule_count = 0;
    int found = 0;

    // 初始化链表
    List_Init(list_play, play_node_t);
    List_Init(list_schedule, schedule_node_t);
    List_Init(list_studio, studio_node_t);

    // 载入所有演出厅信息
    Studio_Srv_FetchAll(list_studio);

    // 获取剧目名称
    if (play_name == NULL) {
        printf("\nPlease input the play name to query: ");
        fflush(stdin);
        gets(name);
        play_name = name;
    }

    // 根据名称获取剧目
    play_count = Play_Srv_FetchByName(list_play, play_name);

    if (play_count == 0) {
        printf("\nNo play found with name '%s'!\n", play_name);
    } else {
        printf("\n==================================================================\n");
        printf("******************** Schedule Query Result ********************\n");
        printf("%5s  %20s  %12s  %12s  %10s  %8s\n",
               "ID", "Play Name", "Studio Name", "Date", "Start Time", "Price");
        printf("------------------------------------------------------------------\n");

        // 遍历每个找到的剧目
        List_ForEach(list_play, play_node) {
            // 根据剧目ID获取演出计划
            List_Free(list_schedule, schedule_node_t);
            schedule_count = Schedule_Srv_FetchByPlay(list_schedule, play_node->data.id);

            // 显示该剧目的所有演出计划
            List_ForEach(list_schedule, schedule_node) {
                // 获取演出厅名称
                studio_node = Studio_Srv_FindByID(list_studio, schedule_node->data.studio_id);
                char *studio_name = (studio_node) ? studio_node->data.name : "Unknown";

                printf("%5d  %20s  %12s  %04d-%02d-%02d  %02d:%02d     %8.2f\n",
                       schedule_node->data.id,
                       play_node->data.name,
                       studio_name,
                       schedule_node->data.date.year,
                       schedule_node->data.date.month,
                       schedule_node->data.date.day,
                       schedule_node->data.time.hour,
                       schedule_node->data.time.minute,
                       schedule_node->data.price);

                found++;
            }
        }

        printf("---------------------- Total Found: %2d -----------------------\n", found);
        printf("******************************************************************\n");
    }

    printf("Press [Enter] key to continue...");
    getchar();

    // 释放链表空间
    List_Destroy(list_play, play_node_t);
    List_Destroy(list_schedule, schedule_node_t);
    List_Destroy(list_studio, studio_node_t);

    return found;
}