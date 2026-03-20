#include "Sale_UI.h"
#include "../Common/List.h"
#include "../Common/common.h"
#include "../Persistence/Sale_Persist.h"
#include "../Service/Sale.h"
#include "../Service/Seat.h"

#include <stdio.h>

void Sale_UI_MgtEnt()
{
    int i;
    char name[30];
    sale_list_t list;
    sale_node_t *pos;

    Pagination_t paging;

    List_Init(list, sale_node_t);
    paging.offset = 0;
    paging.pageSize = SALE_PAGE_SIZE;
    paging.totalRecords = 0;

    Paging_Locate_FirstPage(list, paging);

    char choice;
    do
    {
        printf("\n==================================================================\n");
        printf("********************** Sale **********************\n");
        printf("%5s  %18s  %10s\n", "ID", "Name", "Type");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, sale_node_t, pos, i)
        {
            printf(" a");

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

                break;
            case 's':
            case 'S':
                system(CLEAR);

                break;
            case 'f':
            case 'F':
                system(CLEAR);

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

    } while (choice != 'r' && choice != 'R');

    system(CLEAR);
    List_Destroy(list, sale_node_t);
}

void Sale_UI_ShowSheduler()
{
}

int Sale_UI_Sell_Ticket(ticket_list_t ticket_list, seat_list_t seat_list)
{
    int row, col;
    seat_node_t* seat;
    printf("Sell Ticket\n");
    printf("Please enter the row and column of the seat you want to sell: ");
    scanf("%d %d", &row, &col);

    if ((seat = Seat_Srv_FindByRowCol(seat_list, row, col)) == NULL)
    {
        fprintf(stderr, "Seat not found.\n");
        return -1;
    }

    // Ticket_Srv_FetchBy(seat->data.id
    // Sale_Srv_Add(seat->data.id);
    return 0;

}

void Sale_UI_RefundTicket()
{
    
}