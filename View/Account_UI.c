#include "Account.h"
#include "List.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

int SysLogin()
{
    Account_Srv_InitSys();
    for (int i = 0; i < 3; i++)
    {
        char username[30];
        char password[30];
        printf("Please enter your username: ");
        scanf("%s", username);
        printf("Please enter your password: ");
        scanf("%s", password);
        if (Account_Srv_Verify(username, password) == 1)
        {
            printf("Login successful!\n");
            return 1;
        }
        else
            fprintf(stderr, "Invalid username or password. Please try again.\n");
    }
    fprintf(stderr, "Login failed!\n");
    return 0;
}

int Account_UI_Add(account_list_t list) 
{
    account_t user;
    char* name = malloc(30 * sizeof(char));
    char* pwd = malloc(30 * sizeof(char));
    int type;

    printf("Please enter the username: ");
    scanf("%s", name);
    user.username = name;
    printf("Please enter the password: ");
    fflush(stdin);
    scanf("%s", pwd);
    user.password = pwd;
    user.type = type;

    return 1;
}

int Account_UI_Modify(account_list_t list, char username[])
{
    return 1;
}

int Account_UI_Delete(account_list_t list, char username[])
{
    return 1;
}

int Account_UI_Query(account_list_t list, char username[])
{
    return 1;
}

void Account_UI_MgtEntry()
{
    int i;
    char name[30];
    account_list_t list;
    account_node_t *pos;

    Pagination_t paging;

    List_Init(list, account_node_t);
    paging.offset = 0;
    paging.pageSize = ACCOUNT_PAGE_SIZE;

    Account_Srv_FetchAll(list);
    Paging_Locate_FirstPage(list, paging);

    char choice;
    do
    {
        printf("\n==================================================================\n");
        printf("********************** System Account List **********************\n");
        printf("%5s  %18s  %10s\n", "ID", "Name", "Type");
        printf("------------------------------------------------------------------\n");
        Paging_ViewPage_ForEach(list, paging, account_node_t, pos, i)
        {
            printf("%5d  %18s  %10d\n", pos->data.id, pos->data.username, pos->data.type);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
               paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
        printf("******************************************************************\n");
        printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [M]odify | [Q]uery | [R]eturn");
        printf("\n==================================================================\n");
        printf("Your Choice:");
        fflush(stdin);
        scanf(" %c", &choice);
        fflush(stdin);

        switch (choice)
        {
        case 'a':
        case 'A':
            system(CLEAR);
            if (Account_UI_Add(list))
            {
                paging.totalRecords = Account_Srv_FetchAll(list);
                Paging_Locate_LastPage(list, paging, account_node_t);
            }
            break;
        case 'd':
        case 'D':
            printf("Input the user name:");
            scanf("%s", name);
            if (Account_UI_Delete(list, name))
            {
                paging.totalRecords = Account_Srv_FetchAll(list);
                List_Paging(list, paging, account_node_t);
            }
            break;
        case 'm':
        case 'M':
            printf("Input the user name:");
            scanf("%s", name);
            if (Account_UI_Modify(list, name))
            {
                paging.totalRecords = Account_Srv_FetchAll(list);
                List_Paging(list, paging, account_node_t);
            }
            break;
        case 's':
        case 'S':
            printf("Input the user name:");
            scanf("%s", name);
            paging.totalRecords = Account_Srv_FetchAll(list);
            List_Paging(list, paging, account_node_t);
            break;
        case 'p':
        case 'P':
            system(CLEAR);
            if (!Pageing_IsFirstPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, -1, account_node_t);
            }
            break;
        case 'n':
        case 'N':
            system(CLEAR);
            if (!Pageing_IsLastPage(paging))
            {
                Paging_Locate_OffsetPage(list, paging, 1, account_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');
    system(CLEAR);

    List_Destroy(list, account_node_t);
}