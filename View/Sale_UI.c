#include "../View/Sale_UI.h"
#include "../Common/ListTestDrv.h"
#include "../Common/List.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"
#include "../Service/Play.h"


#include <stdio.h>

void Sale_UI_MgtEntry(void)
{
    char choice;
    play_list_t playList = NULL;
    List_Init(playList, play_node_t);

    Play_Srv_FetchAll(playList);

    while (1)
    {
        printf("\n========== Ticket Sale Management System ==========\n");
        printf("1. Show All Plays\n");
        printf("2. Search Play by ID\n");
        printf("3. View Schedules of a Play\n");
        printf("0. Return to Previous Menu\n");
        printf("==================================================\n");
        printf("Please enter your choice: ");
        scanf(" %c", &choice);

        if (choice == '0')
        {
            break;
        }
        else if (choice == '1')
        {
            play_node_t* pos;
            printf("\n-------- Play List --------\n");
            List_ForEach(playList, pos)
            {
                printf("ID:%d\tName:%s\n", pos->data.id, pos->data.name);
            }
        }
        else if (choice == '2')
        {
            int playID;
            printf("Please enter play ID: ");
            scanf("%d", &playID);

            play_t buf;
            if (Play_Srv_FetchByID(playID, &buf) == 0)
            {
                printf("Found: ID:%d Name:%s\n", buf.id, buf.name);
            }
            else
            {
                printf("Play not found!\n");
            }
        }
        else if (choice == '3')
        {
            int playID;
            printf("Please enter play ID: ");
            scanf("%d", &playID);
            Sale_UI_ShowScheduler(playID);
        }
    }

    List_Destroy(playList, play_node_t);
}


int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList)
{
    int row, col;
    printf("\n======== Ticket Selling Operation ========\n");
    printf("Please enter seat row: ");
    scanf("%d", &row);
    printf("Please enter seat column: ");
    scanf("%d", &col);

    seat_t* seat = Seat_Srv_FindByRowCol(seatList, row, col);
    if (!seat)
    {
        printf("Seat does not exist!\n");
        return -1;
    }

    ticket_node_t* tickNode = Ticket_Srv_FindBySeatID(tickList, seat->id);
    if (!tickNode)
    {
        printf("No ticket available for this seat!\n");
        free(seat);
        return -1;
    }

    if (tickNode->data.status != TICKET_AVL)
    {
        printf("This seat ticket is not available (Sold/Reserved)\n");
        free(seat);
        return -1;
    }

    ticket_t newTick = tickNode->data;
    newTick.status = TICKET_SOLD;
    if (Ticket_Srv_Modify(&newTick) != 0)
    {
        printf("Failed to update ticket status!\n");
        free(seat);
        return -1;
    }

    sale_t sale;
    memset(&sale, 0, sizeof(sale));
    sale.user_id = 1;              
    sale.ticket_id = tickNode->data.id;
    sale.value = tickNode->data.price;
    sale.type = SALE_SELL;

    sale.date.year = 2026;
    sale.date.month = 3;
    sale.date.day = 20;
    sale.time.hour = 16;
    sale.time.minute = 40;
    sale.time.second = 0;

    if (Sale_Srv_Add(&sale) != 0)
    {
        printf("Failed to save sales record!\n");
        free(seat);
        return -1;
    }

    printf("\n=========================================\n");
    printf("       Ticket Sold Successfully!\n");
    printf("Ticket ID: %d\n", tickNode->data.id);
    printf("Seat: Row %d, Column %d\n", row, col);
    printf("Price: %d CNY\n", tickNode->data.price);
    printf("=========================================\n");

    free(seat);
    return 0;
}


void Sale_UI_ShowTicket(int scheduleID)
{
    ticket_list_t tickList = NULL;
    List_Init(tickList, ticket_node_t);

    Ticket_Srv_FetchBySchID(tickList, scheduleID);

    if (List_IsEmpty(tickList))
    {
        printf("No tickets found for this schedule!\n");
        List_Destroy(tickList, ticket_node_t);
        return;
    }

    printf("\n-------- Ticket List --------\n");
    ticket_node_t* pos;
    List_ForEach(tickList, pos)
    {
        const char* status = "Unknown";
        if (pos->data.status == TICKET_AVL) status = "Available";
        else if (pos->data.status == TICKET_SOLD) status = "Sold";
        else if (pos->data.status == TICKET_RESV) status = "Reserved";

        printf("Ticket ID:%d\tSeat ID:%d\tPrice:%d\tStatus:%s\n",
            pos->data.id,
            pos->data.seat_id,
            pos->data.price,
            status);
    }

    char op;
    printf("\n[B]Start selling tickets [R]Return: ");
    scanf(" %c", &op);

    if (op == 'B' || op == 'b')
    {
        schedule_t schBuf;
        Schedule_Srv_FetchByID(scheduleID, &schBuf);

        seat_list_t seatList = NULL;
        List_Init(seatList, seat_node_t);
        Seat_Srv_FetchByRoomID(seatList, schBuf.studio_id);

        Sale_UI_SellTicket(tickList, seatList);

        List_Destroy(seatList, seat_node_t);
    }

    List_Destroy(tickList, ticket_node_t);
}


void Sale_UI_ShowScheduler(int playID)
{
    schedule_list_t schList = NULL;
    List_Init(schList, schedule_node_t);

    Schedule_Srv_FetchByPlay(schList, playID);

    if (List_IsEmpty(schList))
    {
        printf("No schedules found for this play!\n");
        List_Destroy(schList, schedule_node_t);
        return;
    }

    printf("\n-------- Schedule List --------\n");
    schedule_node_t* pos;
    List_ForEach(schList, pos)
    {
        printf("Schedule ID:%d\tDate:%d-%02d-%02d\tStudio ID:%d\n",
            pos->data.id,
            pos->data.date.year,
            pos->data.date.month,
            pos->data.date.day,
            pos->data.studio_id);
    }

    char op;
    printf("\n[T]View tickets of this schedule [R]Return: ");
    scanf(" %c", &op);

    if (op == 'T' || op == 't')
    {
        int schID;
        printf("Please enter schedule ID: ");
        scanf("%d", &schID);
        Sale_UI_ShowTicket(schID);
    }

    List_Destroy(schList, schedule_node_t);
}

