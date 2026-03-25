#include "../View/Ticket_UI.h"
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/Play.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"

static const int TICKET_PAGE_SIZE = 5;

#include <stdio.h>

static void Ticket_UI_ShowTicketsBySchedule(int schedule_id)
{
    ticket_list_t list;
    ticket_node_t *pos;
    int count;

    List_Init(list, ticket_node_t);
    count = Ticket_Srv_FetchBySchID(schedule_id, list);
    if (count <= 0)
    {
        printf("No tickets found for schedule ID %d.\n", schedule_id);
        return;
    }

    printf("Tickets of schedule ID %d (total: %d)\n", schedule_id, count);
    printf("ticket ID      schedule ID      Seat ID      Ticket Price      Ticket status\n");
    List_ForEach(list, pos)
    {
        printf("%6d  %6d     %6d    %6d   %6d\n", pos->data.id, pos->data.schedule_id,
               pos->data.seat_id, pos->data.price, pos->data.status);
    }
}

void Ticket_UI_MgtEntry(int schedule_id)
{
    int i, id;
    int yan_id;
    char choice;
    schedule_t buf;
    play_t data;

    Schedule_Srv_FetchByID(schedule_id, &buf);
    Play_Srv_FetchByID(buf.play_id, &data);

    printf("=======================================================================\n");

    printf("Name          Stduio ID                Date                  Time       \n");
    printf("%s                 %d             %d.%d.%d               %d:%d:%d\n", data.name,
           buf.studio_id, buf.date.year, buf.date.month, buf.date.day, buf.time.hour,
           buf.time.minute, buf.time.second);

    printf("[G]enerating tickets | [R]eproduction of tickets | [Q]uery Tickets\n");
    setbuf(stdin, NULL);
    printf("Your choice:");
    scanf("%c", &choice);
    getchar();

    switch (choice)
    {
    case 'G':
    case 'g': {
        int generated;
        system(CLEAR);
        generated = Ticket_Srv_GenBatch(schedule_id);
        if (generated > 0)
        {
            printf("Generated %d tickets successfully.\n", generated);
            printf("Use [Q] and query by [S]chedule ID %d to list generated tickets.\n",
                   schedule_id);
        }
        else
        {
            printf("Generate tickets failed: no valid seats for this schedule's studio.\n");
            printf("Please go to Studio -> Seat management and initialize/fix seats.\n");
        }
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
        break;
    }
    case 'r':
    case 'R': {
        int generated;
        Ticket_Srv_DeleteBatch(schedule_id);
        generated = Ticket_Srv_GenBatch(schedule_id);
        if (generated > 0)
        {
            printf("Re-generated %d tickets successfully.\n", generated);
            printf("Use [Q] and query by [S]chedule ID %d to list generated tickets.\n",
                   schedule_id);
        }
        else
        {
            printf("Re-generate tickets failed: no valid seats for this schedule's studio.\n");
        }
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
        break;
    }
    case 'q':
    case 'Q':
        Ticket_UI_Query();
        break;
    }
}

void Ticket_UI_Query(void)
{
    char mode;
    int id;

    system(CLEAR);
    printf("==================================================\n");
    printf("******************Find Ticket*********************\n");
    printf("[T] Query by ticket ID | [S] Query by schedule ID | [R]eturn\n");
    printf("Your choice: ");
    scanf("%c", &mode);
    clear_input_buffer();

    if (mode == 's' || mode == 'S')
    {
        printf("Please input schedule ID: ");
        scanf("%d", &id);
        getchar();
        printf("-----------------------------------------------\n");
        Ticket_UI_ShowTicketsBySchedule(id);
        printf("-----------------------------------------------\n");
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
    }
    else if (mode == 't' || mode == 'T')
    {
        printf("Please input ticket ID: ");
        scanf("%d", &id);
        getchar();
        printf("-----------------------------------------------\n");
        Ticket_UI_ShowTicket(id);
        printf("-----------------------------------------------\n");
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
    }
    else if (mode == 'r' || mode == 'R')
    {
        return;
    }
}

int Ticket_UI_ShowTicket(int ticket_id)
{
    ticket_t buf;
    int temp = Ticket_Srv_FetchByID(ticket_id, &buf);
    if (temp == 0)
    {
        printf("the ticket is not exist!\n");
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
    }
    else
    {
        printf(
            "ticket ID      schedule ID      Seat ID      Ticket Price      Ticket status    \n");
        printf("%6d  %6d     %6d    %6d   %6d    \n", buf.id, buf.schedule_id, buf.seat_id,
               buf.price, buf.status);
    }
    return 1;
}

void Ticket_UI_Print(const ticket_t data)
{
    printf("your ticket data : \n");
    printf("Ticket ID      Schedule ID    Saet ID  Price\n");
    printf("%d                  %d           %d      %d    \n", data.id, data.schedule_id,
           data.seat_id, data.price);
    printf("---------------------------------------\n");
    getchar();
}
