#include "Sale_UI.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Persistence/Sale_Persist.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../Service/Play.h"
#include "../Service/Ticket.h"
#include "Account.h"
#include "Schedule.h"
#include "Ticket_UI.h"

#include <stdio.h>

void Sale_UI_MgtEntry()
{
    int id , i = 0;
    char name[30];
    sale_list_t list;
    sale_node_t *pos;

    play_t play;

    Pagination_t paging;

    List_Init(list, sale_node_t);
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = 0;

    paging.totalRecords = Sale_Srv_FetchAll(list);

    Paging_Locate_FirstPage(list, paging);

    char choice;
    do
    {
        system(CLEAR);
        printf("\n==================================================================\n");
        printf("********************** Sale **********************\n");
        printf("%5s  %18s  %10s\n", "ID", "Name", "Type");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, sale_node_t, pos, i)
        {
            printf(" a");
        }
            printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                   paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
            printf("******************************************************************\n");
            printf("[P]revPage | [N]extPage [C]show | [S]earch | [F]ilter |[R]eturn");
            printf("\n==================================================================\n");
            printf("Your Choice:");
            scanf(" %c", &choice);
            clear_input_buffer();

            switch (choice)
            {
            case 'c':
            case 'C':
                system(CLEAR);
                printf("Please enter the ID of the play you want to search: ");
                scanf("%d", &id);
                clear_input_buffer();
                Sale_UI_ShowSheduler(id);
                break;
            case 's':
            case 'S':
                system(CLEAR);
                printf("Please enter the ID of the play you want to search: ");
                scanf("%d", &id);
                clear_input_buffer();
                if (Play_Srv_FetchByID(id, &play))
                    printf("Play found: ID=%d, Name=%s\n", play.id, play.name);
                else
                    printf("Play with ID %d not found.\n", id);
                break;
            case 'f':
            case 'F':
                system(CLEAR);
                // Play_Srv_FetchByName();
                break;
            case 'p':
            case 'P':
                system(CLEAR);
                if (!Pageing_IsFirstPage(paging))
                    Paging_Locate_OffsetPage(list, paging, -1, sale_node_t);
                break;
            case 'n':
            case 'N':
                system(CLEAR);
                if (!Pageing_IsLastPage(paging))
                    Paging_Locate_OffsetPage(list, paging, 1, sale_node_t);
                break;
            }
        }
        while (choice != 'r' && choice != 'R');

    system(CLEAR);
    List_Destroy(list, sale_node_t);
}

void Sale_UI_ShowSheduler(int playID)
{
    Pagination_t paging;
    int i = 0;
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = 0;

    play_t play;
    schedule_list_t list;
    schedule_node_t* pos;

    List_Init(list, schedule_node_t);

    if (!Play_Srv_FetchByID(playID, &play))
    {
        fprintf(stderr, "Play with ID %d not found.\n", playID);
        List_Destroy(list, schedule_node_t);
        return;
    }

    paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
    Paging_Locate_FirstPage(list, paging);

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

        Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
        {
            printf("%5d  %04d-%02d-%02d  %02d:%02d:%02d  %8.2f\n",
                   pos->data.id,
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
        case 'T':
        {
            int scheduleID;
            system(CLEAR);
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
                Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
                Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
            break;
        }

    } while (choice != 'r' && choice != 'R');

    List_Destroy(list, schedule_node_t);
}

int Sale_UI_Sell_Ticket(ticket_list_t ticket_list, seat_list_t seat_list)
{
    int row, col;
    seat_node_t* seat;
    ticket_t ticket;
    sale_t sale;
    account_t clerk;

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
    sale.user_id = clerk.id;
    sale.value = ticket.price;
    sale.type = SALE_SELL;

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

    Sale_Srv_Add(&refound);
    system(CLEAR);
    return;
}
