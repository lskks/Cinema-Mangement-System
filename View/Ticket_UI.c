#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include <stdio.h>
#include <stdlib.h>

void Ticket_UI_MgtEntry(int schedule_id) {
    schedule_t schedule;
    play_t play;
    char choice;
    int result;
    int rtn;

    while (1) {
        system(CLEAR);

        printf("========================================\n");
        printf("          Ticket Generation\n");
        printf("========================================\n\n");

        // a) 调用根据ID获取演出计划函数
        rtn = Schedule_Srv_FetchByID(schedule_id, &schedule);
        if (rtn != 1) {
            printf("Schedule does not exist! (ID: %d)\n", schedule_id);
            printf("Press Enter to return...");
            getchar();
            return;
        }

        // b) 使用剧目ID获取剧目信息
        rtn = Play_Srv_FetchByID(schedule.play_id, &play);

        // c) 在界面中显示信息
        printf("--- Schedule Information ---\n");
        printf("Schedule ID: %d\n", schedule.id);
        if (rtn == 1) {
            printf("Play Name: %s\n", play.name);
            printf("Duration: %d minutes\n", play.duration);
            printf("Base Price: %d\n", play.price);
        }
        printf("Studio ID: %d\n", schedule.studio_id);
        printf("Date: %04d-%02d-%02d\n", schedule.date.year, schedule.date.month, schedule.date.day);
        printf("Time: %02d:%02d:%02d\n", schedule.time.hour, schedule.time.minute, schedule.time.second);

        // d) 显示功能菜单并接收用户输入
        printf("Please choose an operation:\n");
        printf("  G - Generate tickets\n");
        printf("  R - Regenerate tickets\n");
        printf("  Q - Return\n");
        printf("Input your choice: ");
        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice) {
            case 'G':
            case 'g':
                // 调用业务逻辑层批量生成演出票
                result = Ticket_Srv_GenBatch(schedule_id);
                if (result >= 0) {
                    printf("Ticket generation succeeded! Total: %d.\n", result);
                } else {
                    printf("Ticket generation failed!\n");
                }
                break;

            case 'R':
            case 'r':
                // 先删除再生成
                result = Ticket_Srv_DeleteBatch(schedule_id);
                if (result >= 0) {
                    printf("Deleted %d existing tickets.\n", result);
                    result = Ticket_Srv_GenBatch(schedule_id);
                    if (result >= 0) {
                        printf("Ticket regeneration succeeded! Total: %d.\n", result);
                    } else {
                        printf("Regeneration failed!\n");
                    }
                } else {
                    printf("Failed to delete existing tickets!\n");
                }
                break;

            case 'Q':
            case 'q':
                printf("Returning...\n");
                return;

            default:
                printf("Invalid choice! Please try again.\n");
        }

        // e) 等待用户确认后继续
        printf("\nPress Enter to continue...");
        getchar();
    }
}

void Ticket_UI_Query()
{
    ticket_list_t head;
    ticket_node_t* pos = malloc(sizeof(ticket_node_t));

    List_Init(head, ticket_node_t);
    Ticket_Srv_FetchAll(head);

    printf("\n==================================================================\n");
    printf("********************** Ticket Query **********************\n");
    printf("Please enter the id of ticket you want to query: ");
    int id;
    scanf("%d", &id);
    Ticket_UI_ShowTicket(id);

    List_Destroy(head, ticket_node_t);
}

int Ticket_UI_ShowTicket(int ticket_id)
{
    ticket_t* ticket;
    if (Ticket_Srv_FetchByID(ticket_id, ticket) != 1)
    {
        fprintf(stderr, "Cann't find the ticket %d\n", ticket_id);
        return 0;
    }
    printf("The infomation of the ticket: \n");
    printf("ID: %d\nPrice: %d\nSchedule ID:%d\nSeat ID: %d, Status: %d", ticket->id, ticket->price, ticket->schedule_id, ticket->seat_id, ticket->status);
    system(CLEAR);
    return 1;
}
