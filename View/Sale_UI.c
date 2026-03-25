#include "Sale_UI.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Persistence/Sale_Persist.h"
#include "../Service/Play.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../Service/Ticket.h"
#include "Account.h"
#include "Schedule.h"
#include "Studio.h"
#include "Ticket_UI.h"

#include <stdio.h>

void Sale_UI_MgtEntry()
{
    int i, id;
    char choice;
    char play_name[31];
    schedule_list_t schedule_list;
    schedule_node_t *schedule_node;
    play_list_t play_list;
    play_node_t *play_node;
    Pagination_t paging;
    int found = 0;
    play_t play;

    // 设置分页显示页的大小
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;

    do
    {
        // 调用List_Init宏初始化链表
        List_Init(schedule_list, schedule_node_t);
        List_Init(play_list, play_node_t);

        // 获取全部剧目
        Play_Srv_FetchAll(play_list);

        printf("\n==================================================================\n");
        printf("********************** Schedule List **************************\n");
        printf("%-5s %-20s %-12s %-12s %-10s %-8s\n", "ID", "Play Name", "Studio Name", "Date",
               "Start Time", "Price");
        printf("------------------------------------------------------------------\n");

        // 显示演出计划
        paging.totalRecords = Schedule_Srv_FetchAll(schedule_list);
        Paging_Locate_FirstPage(schedule_list, paging);

        Paging_ViewPage_ForEach(schedule_list, paging, schedule_node_t, schedule_node, i)
        {
            play_node = Play_Srv_FindByID(play_list, schedule_node->data.play_id);
            char *play_name_display = (play_node) ? play_node->data.name : "Unknown";

            printf("%-5d %-20s %-12d %04d-%02d-%02d %02d:%02d %8.2f\n", schedule_node->data.id,
                   play_name_display, schedule_node->data.studio_id, schedule_node->data.date.year,
                   schedule_node->data.date.month, schedule_node->data.date.day,
                   schedule_node->data.time.hour, schedule_node->data.time.minute,
                   schedule_node->data.price);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[C]hoose Schedule | [S]earch by Play Name | [P]revPage | [N]extPage | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        scanf("%c", &choice);
        clear_input_buffer();

        switch (choice)
        {
        case 'c':
        case 'C':
            // 选择演出计划
            printf("Input Schedule ID:");
            int schedule_id;
            scanf("%d", &schedule_id);
            Sale_UI_ShowTicket(schedule_id);
            break;
        case 's':
        case 'S':
            // 查询剧目名字
            printf("Input Play ID:");
            fflush(stdin);
            scanf("%d", &id);

            // 根据剧目名称获取演出计划
            List_Free(schedule_list, schedule_node_t);
            found = Schedule_Srv_FetchByID(id, &schedule_node->data);

            if (found == 0)
            {
                printf("No schedule found for play: %d\n", id);
                printf("Press Enter to continue...");
                getchar();
                break;
            }

            // 显示查询结果
            printf("\n========== Search Results ==========\n");
            List_ForEach(schedule_list, schedule_node)
            {
                play_node = Play_Srv_FindByID(play_list, schedule_node->data.play_id);
                printf("ID: %d, Play: %s, Date: %04d-%02d-%02d, Time: %02d:%02d, Price: %.2f\n",
                       schedule_node->data.id, play_node ? play_node->data.name : "Unknown",
                       schedule_node->data.date.year, schedule_node->data.date.month,
                       schedule_node->data.date.day, schedule_node->data.time.hour,
                       schedule_node->data.time.minute, schedule_node->data.price);
            }

            printf("Choose Schedule ID to buy ticket:");
            int chosen_id;
            scanf("%d", &chosen_id);
            Sale_UI_ShowTicket(chosen_id);
            break;
        case 'p':
        case 'P':
            system(CLEAR);
            if (!Pageing_IsFirstPage(paging))
                Paging_Locate_OffsetPage(schedule_list, paging, -1, schedule_node_t);
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(schedule_list, paging, 1, schedule_node_t);
            break;
        }
    } while (choice != 'r' && choice != 'R');

    List_Destroy(schedule_list, schedule_node_t);
}

void Sale_UI_ShowTicket(int schedule_id)
{
    int i;
    char choice;
    ticket_list_t ticket_list;
    ticket_node_t *ticket_node;
    schedule_t schedule;
    studio_t studio;
    seat_list_t seat_list;
    seat_node_t *seat_node;
    Pagination_t paging;

    // 获取演出计划信息
    if (!Schedule_Srv_FetchByID(schedule_id, &schedule))
    {
        printf("Schedule not found!\n");
        return;
    }

    // 获取演出厅信息
    if (!Studio_Srv_FetchByID(schedule.studio_id, &studio))
    {
        printf("Studio not found!\n");
        return;
    }

    // 载入座位信息
    List_Init(seat_list, seat_node_t);
    Seat_Srv_FetchByRoomID(seat_list, schedule.studio_id);

    // 设置分页
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;

    do
    {
        // 获取票的数据
        List_Init(ticket_list, ticket_node_t);
        paging.totalRecords = Ticket_Srv_SelBySchedule( schedule_id, ticket_list);
        Paging_Locate_FirstPage(ticket_list, paging);

        printf("\n==================================================================\n");
        printf("********************** Ticket List ******************************\n");
        printf("%-5s %-5s %-10s %-10s\n", "ID", "Seat ID", "Price", "Status");
        printf("------------------------------------------------------------------\n");

        // 显示所有的票
        Paging_ViewPage_ForEach(ticket_list, paging, ticket_node_t, ticket_node, i)
        {
            char *status_str;
            switch (ticket_node->data.status)
            {
            case TICKET_AVL:
                status_str = "Available";
                break;
            case TICKET_SOLD:
                status_str = "Sold";
                break;
            case TICKET_RESV:
                status_str = "Reserved";
                break;
            default:
                status_str = "Unknown";
            }
            printf("%-5d  %-5d %-10.2d %-10s\n", ticket_node->data.id,
                   ticket_node->data.seat_id, ticket_node->data.price, status_str);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[B]uy Ticket | [P]revPage | [N]extPage | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        fflush(stdin);
        scanf("%c", &choice);
        fflush(stdin);

        switch (choice)
        {
        case 'b':
        case 'B':
            // 售票
            Sale_UI_SellTicket(ticket_list, seat_list);
            break;

        case 'p':
        case 'P':
            // 前一页
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(ticket_list, paging, -1, ticket_node_t);
            }
            break;

        case 'n':
        case 'N':
            // 下一页
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(ticket_list, paging, 1, ticket_node_t);
            }
            break;
        }

        // 销毁链表
        List_Destroy(ticket_list, ticket_node_t);

    } while (choice != 'r' && choice != 'R');

    List_Destroy(seat_list, seat_node_t);
}

void Sale_UI_ShowScheduler(int playID)
{
    Pagination_t paging;
    int i = 0;
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = 0;

    play_t play;
    schedule_list_t schedule_list;
    schedule_node_t *pos;

    List_Init(schedule_list, schedule_node_t);

    if (!Play_Srv_FetchByID(playID, &play))
    {
        fprintf(stderr, "Play with ID %d not found.\n", playID);
        List_Destroy(schedule_list, schedule_node_t);
        return;
    }

    paging.totalRecords = Schedule_Srv_FetchByPlay(schedule_list, playID);
    Paging_Locate_FirstPage(schedule_list, paging);

    char choice;
    do
    {
        system(CLEAR);
        printf("\n==================================================================\n");
        printf("********************** Schedule **********************\n");
        printf("Play: %s (ID: %d)\n", play.name, play.id);
        printf("%5s  %12s  %10s  %8s\n", "ID", "Date", "Time", "Price");
        printf("------------------------------------------------------------------\n");

        if (paging.totalRecords == 0)
        {
            printf("No schedules found for this play.\n");
        }

        Paging_ViewPage_ForEach(schedule_list, paging, schedule_node_t, pos, i)
        {
            printf("%5d  %04d-%02d-%02d  %02d:%02d:%02d  %8.2f\n", pos->data.id,
                   pos->data.date.year, pos->data.date.month, pos->data.date.day,
                   pos->data.time.hour, pos->data.time.minute, pos->data.time.second,
                   pos->data.price);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | Show [T]icket | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice)
        {
        case 't':
        case 'T': {
            int scheduleID;
            // system(CLEAR);
            printf("Please enter schedule ID: ");
            scanf("%d", &scheduleID);
            clear_input_buffer();
            Ticket_UI_MgtEntry(scheduleID);
            break;
        }
        case 'p':
        case 'P':
            system(CLEAR);
            if (!Pageing_IsFirstPage(paging))
                Paging_Locate_OffsetPage(schedule_list, paging, -1, schedule_node_t);
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(schedule_list, paging, 1, schedule_node_t);
            break;
        }

    } while (choice != 'r' && choice != 'R');

    List_Destroy(schedule_list, schedule_node_t);
}

int Sale_UI_SellTicket(ticket_list_t ticket_list, seat_list_t seat_list)
{
    int row, col;
    seat_node_t *seat;
    ticket_t ticket;
    sale_t sale;

    printf("Sell Ticket\n");
    printf("Please enter the row and column of the seat you want to sell: ");
    scanf("%d %d", &row, &col);
    clear_input_buffer();

    if ((seat = Seat_Srv_FindByRowCol(seat_list, row, col)) == NULL)
    {
        fprintf(stderr, "Seat not found.\n");
        return -1;
    }

    if (Ticket_Srv_FetchByID(seat->data.id, &ticket) == 0)
    {
        fprintf(stderr, "Ticket not found\n");
        return -1;
    }

    if (ticket.status != TICKET_AVL)
    {
        printf("Ticket %d has been sold", ticket.id);
        return -1;
    }

    ticket.status = TICKET_SOLD;
    Ticket_Srv_Modify(&ticket);

    sale.ticket_id = ticket.id;
    sale.date = DateNow();
    sale.time = TimeNow();
    sale.user_id = gl_CurUser.id;
    sale.value = ticket.price;
    sale.type = SALE_SELL;
    sale.id = seat->data.id;

    Sale_Srv_Add(&sale);

    return 0;
}

void Sale_UI_RefundTicket()
{
    printf("Refund Ticket\n");
    printf("Please enter the ID of the ticket you want to refund: ");
    int id;
    scanf("%d", &id);
    clear_input_buffer();
    ticket_t ticket;
    if (Ticket_Srv_FetchByID(id, &ticket) == 0)
    {
        fprintf(stderr, "Ticket with ID %d not found.\n", id);
        printf("Press Enter to return...");
        getchar();
        system(CLEAR);
        return;
    }

    if (ticket.status == TICKET_AVL)
    {
        system(CLEAR);
        return;
    }

    ticket.status = TICKET_RESV;

    sale_t refound;
    refound.ticket_id = ticket.id;
    refound.user_id = gl_CurUser.id;
    refound.type = SALE_REFOUND;
    refound.value = -ticket.price;

    Ticket_Srv_Modify(&ticket);
    Sale_Srv_Add(&refound);
    system(CLEAR);
    return;
}
