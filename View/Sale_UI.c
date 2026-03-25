#include "Sale_UI.h"
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/Play.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "Schedule.h"
#include "Ticket_UI.h"
#include <stdio.h>
#include <time.h>
static const int SALE_PAGE_SIZE = 5;
static const int SALESANALYSIS_PAGE_SIZE = 5;
static const int TICKET_PAGE_SIZE = 10;

void List_Foreach(ticket_list_t list, int id);

char* StatusToStr(int status)
{
    switch (status)
    {
    case 0:
        return "Available";
    case 1:
        return "Sold";
    case 9:
        return "Reversed";
    default:
        return "Unknown";
    }
}

void Sale_UI_MgtEntry()
{

    system("clear");
    char choice;
    char name[20];
    int i;
    int play_id;
    play_node_t *pos;

    play_list_t list;
    play_list_t list_fetch;
    List_Init(list, play_node_t);
    List_Init(list_fetch, play_node_t);

    Pagination_t paging;
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    Play_Srv_FetchAll(list_fetch);
    paging.totalRecords = Play_Srv_FetchAll(list);
    Paging_Locate_FirstPage(list, paging);

    do
    {
        printf("\n================================================================================="
               "=====\n");
        printf("***************************** Projection Play List "
               "***********************************\n");

        printf("%5s %15s %5s %10s %3s %3s %10s %10s     %3s\n", "ID", "Name", "Type", "Area",
               "Rating", "Duration", "Start Date", "End Date", "Price");
        printf("-----------------------------------------------------------------------------------"
               "---\n");

        Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i)
        {
            printf("%5d %15s %5d %10s %6d %6d %7d%3d%3d %4d%3d%3d     %4d\n", pos->data.id,
                   pos->data.name, pos->data.type, pos->data.area, pos->data.rating,
                   pos->data.duration, pos->data.start_date.year, pos->data.start_date.month,
                   pos->data.start_date.day, pos->data.end_date.year, pos->data.end_date.month,
                   pos->data.end_date.day, pos->data.price);
        }

        printf("----------------Total Recoeds: %2d---------------------------Page %2d/%2d   "
               "-----------\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

        printf("***********************************************************************************"
               "***\n");

        printf("[C]show Schedule | [S]find play name | [P]rev | [N]ext | [R]eturn \n");
        printf("===================================================================================\n");
        printf("please input your choice : ");
        setbuf(stdin, NULL);
        scanf("%c", &choice);
        getchar();
        switch (choice)
        {
        case 'c':
        case 'C':
            printf("please input play_id  :");
            scanf("%d", &play_id);
            getchar();
            Sale_UI_ShowScheduler(play_id);
            break;
        case 's':
        case 'S':
            printf("please input the Play_ name :");
            setbuf(stdin, NULL);
            scanf("%19s", name);
            clear_input_buffer();
            system(CLEAR);
            Play_Srv_FetchByName(list_fetch, name);
            Play_Srv_Print(list_fetch, name);
            break;

        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
            }
            break;
        case 'P':
        case 'p':
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
            }
            break;
        }

    } while (choice != 'r' && choice != 'R');
    system(CLEAR);
}

void Sale_UI_ShowScheduler(int play_id)
{
    system("clear");
    int i;
    char choice;
    int schedule_id;

    schedule_list_t list;
    schedule_node_t *pos;
    Pagination_t paging;

    List_Init(list, schedule_node_t);

    paging.offset = 0;
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;

    paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
    Paging_Locate_FirstPage(list, paging);
    studio_t buf;
    do
    {
        printf("==================================================================================="
               "================\n");
        printf("*****************************************Schedule  "
               "List*******************************************\n");
        printf("-----------------------------------------------------------------------------------"
               "----------------\n");

        printf(" Schedule ID     Play ID      Stdio ID       Year     Month     Day     Hour     Minute     Second     Seat Count\n");
        printf("-----------------------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
        {
            printf("         %3d         %3d          %3d             %3d.%2d.%2d           "
                   "%3d:%3d:%3d               %d  \n",
                   pos->data.id, pos->data.play_id, pos->data.studio_id, pos->data.date.year,
                   pos->data.date.month, pos->data.date.day, pos->data.time.hour,
                   pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
        }
        printf("-----totalRecords : %2d  -----------------------------Page %2d/%2d "
               "------------------------------------\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

        printf("[T] Show Ticket | [N]ext | [P]rev | [R]eturn \n");

        printf("Your choice : ");

        scanf("%c", &choice);
        getchar();

        switch (choice)
        {
        case 'T':
        case 't':
            printf("Please input Schedule ID:");
            setbuf(stdin, NULL);
            scanf("%d", &schedule_id);
            getchar();
            Sale_UI_ShowTicket(schedule_id);
            break;
        case 'N':
        case 'n':
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
            }
            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
            }
            break;
        }

    } while (choice == 'r' && choice == 'R');
}

int Sale_UI_ShowTicket(int schedule_id)
{
    seat_list_t list;
    List_Init(list, seat_node_t);
    int studio_ID;

    printf("please input the studio ID :");
    scanf("%d", &studio_ID);
    getchar();
    int seat = Seat_Srv_FetchValidByRoomID(list, studio_ID);

    int i = 1;
    char choice;
    ticket_node_t *pos;
    Pagination_t paging;

    paging.offset = 0;
    paging.pageSize = TICKET_PAGE_SIZE;
    ticket_list_t ticket_list;

    List_Init(ticket_list, ticket_node_t);

    paging.totalRecords = Ticket_Srv_FetchBySchID( schedule_id, ticket_list);
    Paging_Locate_FirstPage(ticket_list, paging);
    studio_t buf;
    do
    {
        system(CLEAR);
        printf("==================================================================================="
               "=\n");
        printf("*****************************************Ticket************************************"
               "*\n");
        printf("-----------------------------------------------------------------------------------"
               "-\n");
        printf("Ticket ID          Schedule ID         Seat ID       Price             Ticket "
               "Status\n");
        Paging_ViewPage_ForEach(ticket_list, paging, ticket_node_t, pos, i)
        {
            printf("  %5d               %5d            %5d      %5d                     %5s\n",
                   pos->data.id, pos->data.schedule_id, pos->data.seat_id, pos->data.price,
                   StatusToStr(pos->data.status));
        }

        printf("------Total Records  : %2d ------------------------Page %2d / "
               "%2d----------------------\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        Studio_Srv_FetchByID(studio_ID, &buf);

        printf("[N]ext | [P]rev | [S]ale Ticket | [R]eturn\n");
        printf("Your choice:");
        choice = getchar();
        getchar();

        switch (choice)
        {

        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(ticket_list, paging, 1, ticket_node_t);
            }

            break;
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(ticket_list, paging, -1, ticket_node_t);
            }
            break;
        case 'S':
        case 's':
            Sale_UI_SellTicket(ticket_list, list);
            break;
        }

    } while (choice != 'R' && choice != 'r');
    system(CLEAR);
    return 1;
}

int Sale_UI_SellTicket(ticket_list_t list_t, seat_list_t list_s)
{
    seat_node_t *seat = NULL;
    sale_t data_t;
    int id;
    while (1)
    {
        printf("please input the ticket ID you want to buy :");
        scanf("%d", &id);
        clear_input_buffer();
        seat = Seat_Srv_FindByID(list_s, id);

        if (NULL == seat)
        {
            printf("the seat is not exit!\n");
            continue;
        }

        if (seat->data.status == '~')
        {
            printf("the seat is broken!\n\n");
        }
        else if (seat->data.status == ' ')
        {
            printf("the seat is empty!!\n");
        }
        else
        {
            break;
        }
    }

    ticket_t buf;
    account_t buf_a;

    Ticket_Srv_FetchByTicketID(seat->data.id, &buf);
    if (buf.status == 1)
    {
        printf("the ticket is Sale!\n");
        return 0;
    }
    else
    {
        data_t.ticket_id = buf.id;
        List_Foreach(list_t, buf.id);
        buf.status = 1;
        Ticket_Srv_Modify(&buf);
        while (1)
        {
            printf("please input the saler ID:");
            scanf("%d", &data_t.user_id);
            if (!Account_Srv_FetchByID(data_t.user_id, &buf_a))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        data_t.type = 1;
        data_t.value = buf.price;
        struct tm *p;
        time_t timep;
        time(&timep);
        p = localtime(&timep);
        data_t.date.year = p->tm_year + 1900;
        data_t.date.month = p->tm_mon + 1;
        data_t.date.day = p->tm_mday;
        data_t.time.hour = p->tm_hour;
        data_t.time.minute = p->tm_min;
        data_t.time.second = p->tm_sec;
        Sale_Srv_Add(&data_t);

        Ticket_UI_Print(buf);
    }
    return 1;
}

void Sale_UI_ReturnTicket()
{
    // if (gl_CurUser.type == USR_ADMIN)
    // {
    //     printf("you can't join in there!please input the [Enter]");
    //     getchar();
    //     return;
    // }

    int id;
    ticket_t buf;
    sale_t refound;
    printf("Please input the ticket ID:");
    scanf("%d", &id);
    if (Ticket_Srv_FetchByTicketID(id, &buf) == 1)
    {
        if (buf.status == TICKET_AVL)
        {
            buf.status = TICKET_SOLD;
            Ticket_Srv_Modify(&buf); 

            refound.ticket_id = buf.id;
            refound.value = buf.price;
            refound.type = -1;
            struct tm *p;
            time_t timep;
            time(&timep);
            p = localtime(&timep);
            refound.date.year = p->tm_year + 1900;
            refound.date.month = p->tm_mon + 1;
            refound.date.day = p->tm_mday;
            refound.time.hour = p->tm_hour;
            refound.time.minute = p->tm_min;
            refound.time.second = p->tm_sec;
            printf("please input Salesperson ID:");
            scanf("%d", &refound.user_id);
            getchar();
            Sale_Srv_Add(&refound);
        }
        else
        {
            printf("This ticket has not been sold!\n");
        }
    }
    else
    {
        printf("Ticket data error!\n");
    }
}

void List_Foreach(ticket_list_t list, int id)
{
    while (1)
    {
        if (list->data.id == id)
        {
            list->data.status = 1;
            break;
        }
        else
        {
            list = list->next;
        }
    }
}