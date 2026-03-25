#include "../View/Ticket_UI.h"
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/Schedule.h"
#include "../Service/Ticket.h"
#include "../Service/Play.h"

static const int TICKET_PAGE_SIZE = 5;

#include <stdio.h>

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
    case 'g':
        system(CLEAR);
        Ticket_Srv_GenBatch(schedule_id);
        printf("Successlly!\n");
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
        break;
    case 'r':
    case 'R':
        Ticket_Srv_DeleteBatch(schedule_id);
        Ticket_Srv_GenBatch(schedule_id);
        printf("Successlly!\n");
        printf("Please enter any key to continue...");
        getchar();
        system(CLEAR);
        break;
    case 'q':
    case 'Q':
        Ticket_UI_Query();
        break;
    }
}

void Ticket_UI_Query(void)
{
    printf("==================================================\n");
    printf("******************Find Ticket*********************\n");
    printf("please input the id of the ticket :");

    int id;
    scanf("%d", &id);
    getchar();

    Ticket_UI_ShowTicket(id);
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
