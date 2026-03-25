#include "../Persistence/Play_Persist.h"
#include "Studio.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int Play_Srv_Add(play_t *data)
{
    return Play_Perst_Insert(data);
}

int Play_Srv_Modify(const play_t *data)
{
    return Play_Perst_Update(data);
}

int Play_Srv_DeleteByID(int ID)
{
    return Play_Perst_RemByID(ID);
}

int Play_Srv_FetchByID(int ID, play_t *buf)
{
    return Play_Perst_SelectByID(ID, buf);
}

int Play_Srv_FetchAll(play_list_t list)
{
    return Play_Perst_SelectAll(list);
}

play_node_t *Play_Srv_FindByID(play_list_t list, int ID)
{
    assert(NULL != list);
    play_node_t *ptr = list->next;
    while (ptr != list)
    {
        if (ptr->data.id == ID)
            return ptr;
        else
            ptr = ptr->next;
    }

    return NULL;
}

int Play_Srv_FetchByName(play_list_t list, char condt[])
{
    return Play_Perst_SelectByName(list, condt);
}

void Play_Srv_Print(play_list_t list, char s[])
{
    play_list_t temp;
    temp = list;

    list = list->next;
    if (list != temp)
    {
        printf("***************** %s  Play List **************************\n",s);
        printf("Play  name    Play ID   Type     Area      Rating     Duration     Start_date      "
               "     End_date         Price     \n");
    }
    else
    {
        printf("****************Nothing************************\n");
        return;
    }
    while (list != temp)
    {
        printf("%s    %d       %d     %s        %d            %d     %d.%d.%d               "
               "%d.%d.%d          %d\n",
               list->data.name, list->data.id, list->data.type, list->data.area, list->data.rating,
               list->data.duration, list->data.start_date.year, list->data.start_date.month,
               list->data.start_date.day, list->data.end_date.year, list->data.end_date.month,
               list->data.end_date.day, list->data.price);
        list = list->next;
    }
    setbuf(stdin, NULL);
    printf("----------------------------------------------------------------------\n");
    printf("please press Enter return!\n");
    getchar();

    return;
}