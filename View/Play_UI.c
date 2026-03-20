#include "../View/Play_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "Schedule_UI.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

static const int PLAY_PAGE_SIZE = 5;

static const char *Play_UI_TypeToStr(play_type_t type)
{
    switch (type)
    {
    case PLAY_TYPE_FILE:
        return "Drama";
    case PLAY_TYPE_OPEAR:
        return "Opera";
    case PLAY_TYPE_CONCERT:
        return "Concert";
    default:
        return "Unknown";
    }
}

static const char *Play_UI_RatingToStr(play_rating_t rating)
{
    switch (rating)
    {
    case PLAY_RATE_CHILD:
        return "Child";
    case PLAY_RATE_TEENAGE:
        return "Teen";
    case PLAY_RATE_ADULT:
        return "Adult";
    default:
        return "Unknown";
    }
}

void Play_UI_MgtEntry(void)
{
    int i, id;
    char choice;
    play_list_t list;
    play_node_t *pos;
    Pagination_t paging;

    List_Init(list, play_node_t);
    paging.offset = 0;
    paging.pageSize = PLAY_PAGE_SIZE;
    paging.totalRecords = Play_Srv_FetchAll(list);
    Paging_Locate_FirstPage(list, paging);

    do
    {
        printf("\n==================================================================\n");
        printf("************************* Play Management *************************\n");
        printf("%4s  %-20s  %-8s  %-8s  %-6s  %-5s\n", "ID", "Name", "Type", "Rating", "Dur", "Price");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i)
        {
            printf("%4d  %-20s  %-8s  %-8s  %-6d  %-5d\n",
                   pos->data.id,
                   pos->data.name,
                   Play_UI_TypeToStr(pos->data.type),
                   Play_UI_RatingToStr(pos->data.rating),
                   pos->data.duration,
                   pos->data.price);
        }
        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]earch | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice)
        {
        case 'a':
        case 'A':
            system(CLEAR);
            if (Play_UI_Add())
            {
                paging.totalRecords = Play_Srv_FetchAll(list);
                Paging_Locate_LastPage(list, paging, play_node_t);
            }
            break;
        case 'd':
        case 'D':
            system(CLEAR);
            printf("Input the play ID to delete: ");
            scanf("%d", &id);
            clear_input_buffer();
            if (Play_UI_Delete(id))
            {
                paging.totalRecords = Play_Srv_FetchAll(list);
                List_Paging(list, paging, play_node_t);
            }
            break;
        case 'u':
        case 'U':
            system(CLEAR);
            printf("Input the play ID to update: ");
            scanf("%d", &id);
            clear_input_buffer();
            if (Play_UI_Modify(id))
            {
                paging.totalRecords = Play_Srv_FetchAll(list);
                List_Paging(list, paging, play_node_t);
            }
            break;
        case 's':
        case 'S':
            system(CLEAR);
            printf("Input the play ID to search: ");
            scanf("%d", &id);
            clear_input_buffer();
            if (Play_UI_Query(id))
            {
                system(CLEAR);
                Schedule_UI_MgtEntry(id);
            }
            break;
        case 'p':
        case 'P':
            system(CLEAR);
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
            }
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');

    system(CLEAR);
    List_Destroy(list, play_node_t);
}

int Play_UI_Add(void)
{
    play_t data;
    memset(&data, 0, sizeof(play_t));

    printf("=======================================================\n");
    printf("********************** Add New Play *******************\n");
    printf("Play Name: ");
    scanf("%30s", data.name);
    printf("Play Type (1-Drama 2-Opera 3-Concert): ");
    scanf("%d", (int *)&data.type);
    printf("Play Area: ");
    scanf("%8s", data.area);
    printf("Play Rating (1-Child 2-Teenage 3-Adult): ");
    scanf("%d", (int *)&data.rating);
    printf("Duration (minutes): ");
    scanf("%d", &data.duration);
    printf("Start Date (YYYY MM DD): ");
    scanf("%d %d %d", &data.start_date.year, &data.start_date.month, &data.start_date.day);
    printf("End Date (YYYY MM DD): ");
    scanf("%d %d %d", &data.end_date.year, &data.end_date.month, &data.end_date.day);
    printf("Price: ");
    scanf("%d", &data.price);
    clear_input_buffer();
    printf("=======================================================\n");

    if (Play_Srv_Add(&data))
    {
        printf("Play added successfully! ID=%d\n", data.id);
        return 1;
    }

    printf("Failed to add play.\n");
    return 0;
}

int Play_UI_Modify(int id)
{
    play_t buf;
    if (!Play_Srv_FetchByID(id, &buf))
    {
        printf("Play does not exist.\n");
        return 0;
    }

    printf("=======================================================\n");
    printf("********************** Update Play ********************\n");
    printf("Play ID: %d\n", id);
    printf("Name [%s]: ", buf.name);
    scanf("%30s", buf.name);
    printf("Type [%d] (1-Drama 2-Opera 3-Concert): ", buf.type);
    scanf("%d", (int *)&buf.type);
    printf("Area [%s]: ", buf.area);
    scanf("%8s", buf.area);
    printf("Rating [%d] (1-Child 2-Teenage 3-Adult): ", buf.rating);
    scanf("%d", (int *)&buf.rating);
    printf("Duration [%d]: ", buf.duration);
    scanf("%d", &buf.duration);
    printf("Start Date [%d-%d-%d] (YYYY MM DD): ",
           buf.start_date.year, buf.start_date.month, buf.start_date.day);
    scanf("%d %d %d", &buf.start_date.year, &buf.start_date.month, &buf.start_date.day);
    printf("End Date [%d-%d-%d] (YYYY MM DD): ",
           buf.end_date.year, buf.end_date.month, buf.end_date.day);
    scanf("%d %d %d", &buf.end_date.year, &buf.end_date.month, &buf.end_date.day);
    printf("Price [%d]: ", buf.price);
    scanf("%d", &buf.price);
    clear_input_buffer();
    printf("=======================================================\n");

    if (Play_Srv_Modify(&buf))
    {
        printf("Play updated successfully.\n");
        return 1;
    }

    printf("Failed to update play.\n");
    return 0;
}

int Play_UI_Delete(int id)
{
    char confirm;
    printf("Are you sure to delete play ID %d? (y/n): ", id);
    scanf(" %c", &confirm);
    clear_input_buffer();

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Delete cancelled.\n");
        return 0;
    }

    if (Play_Srv_DeleteByID(id))
    {
        printf("Play deleted successfully.\n");
        return 1;
    }

    printf("Failed to delete play (may not exist).\n");
    return 0;
}

int Play_UI_Query(int id)
{
    play_t data;
    if (!Play_Srv_FetchByID(id, &data))
    {
        printf("No play found with ID: %d\n", id);
        return 0;
    }

    printf("=======================================================\n");
    printf("Play Detail\n");
    printf("-------------------------------------------------------\n");
    printf("ID       : %d\n", data.id);
    printf("Name     : %s\n", data.name);
    printf("Type     : %s\n", Play_UI_TypeToStr(data.type));
    printf("Area     : %s\n", data.area);
    printf("Rating   : %s\n", Play_UI_RatingToStr(data.rating));
    printf("Duration : %d minutes\n", data.duration);
    printf("Date     : %d-%02d-%02d to %d-%02d-%02d\n",
           data.start_date.year, data.start_date.month, data.start_date.day,
           data.end_date.year, data.end_date.month, data.end_date.day);
    printf("Price    : %d\n", data.price);
    printf("=======================================================\n");
    return 1;
}
