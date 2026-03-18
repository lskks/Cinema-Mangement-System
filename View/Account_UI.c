#include "Account.h"
#include "List.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int SysLogin()
{
    Account_Srv_InitSys();
    for (int i = 0; i < 3; i++)
    {
        char username[30];
        char password[30];
        printf("Please enter your username: ");
        scanf("%29s", username);
        printf("Please enter your password: ");
        scanf("%29s", password);
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
    account_t user = {0};
    account_node_t *newNode = NULL;
    system(CLEAR);
    printf("===================================================================\n");
    printf("********************** Add New System Account **********************\n");

    printf("Please enter the username: ");
    scanf("%29s", user.username);

    if (Account_Srv_FindByUserName(list, user.username) != NULL)
    {
        fprintf(stderr, "Username already exists. Please choose a different username.\n");
        return 0;
    }
    
    printf("Please enter the password: ");
    scanf("%29s", user.password);
    user.username[ACCOUNT_NAME_LEN - 1] = '\0';
    user.password[ACCOUNT_PWD_LEN - 1] = '\0';
    user.type = USR_CLERK;

    if (!Account_Srv_Add(&user))
    {
        fprintf(stderr, "Add account failed.\n");
        return 0;
    }

    newNode = (account_node_t *)malloc(sizeof(account_node_t));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed, fallback to full list refresh.\n");
        Account_Srv_FetchAll(list);
        return 1;
    }

    newNode->data = user;
    List_AddTail(list, newNode);

    return 1;
}

int Account_UI_Modify(account_list_t list, char username[])
{
    account_node_t* user;
    account_node_t* pos;
    
    if ((user = Account_Srv_FindByUserName(list, username)) == NULL)
    {
        fprintf(stderr, "Username not found.\n");
        return 0;
    }

    printf("Please enter the new password: ");
    char newpwd[30];
    scanf("%29s", newpwd);
    newpwd[ACCOUNT_PWD_LEN - 1] = '\0';

    if (Account_Srv_Modify(&user->data) == 0)
    {
        fprintf(stderr, "Update account failed.\n");
        return 0;
    }

    List_ForEach(list, pos)
    {
        if (strcmp(pos->data.username, username) == 0)
        {
            strncpy(pos->data.password, newpwd, ACCOUNT_PWD_LEN - 1);
            pos->data.password[ACCOUNT_PWD_LEN - 1] = '\0';
            break;
        }
    }
    
    return 1;
}

int Account_UI_Delete(account_list_t list, char username[])
{
    account_node_t* user;
    account_node_t* pos;
    
    if ((user = Account_Srv_FindByUserName(list, username)) == NULL)
    {
        fprintf(stderr, "Username not found.\n");
        return 0;
    }

    
    if (Account_Srv_DeleteByID(user->data.id) == 0)
    {
        fprintf(stderr, "Delete account failed.\n");
        return 0;
    }

    List_ForEach(list, pos)
    {
        if (strcmp(pos->data.username, username) == 0)
        {
            List_DelNode(pos);
            free(pos);
            break;
        }
    }
    
    return 1;;
}

int Account_UI_Query(account_list_t list, char username[])
{
    account_node_t* user;
    
    if ((user = Account_Srv_FindByUserName(list, username)) == NULL)
    {
        fprintf(stderr, "Username not found.\n");
        return 0;
    }

    printf("%5s  %18s  %10s\n", "ID", "Name", "Type");
    printf("%5d  %18s  %10d\n", user->data.id, user->data.username, user->data.type);
    
    return 1;
}

void Account_UI_MgtEntry()
{
    int i;
    char name[30];
    account_list_t list;
    account_node_t *pos;

    Pagination_t paging;

    Account_Srv_InitSys();

    List_Init(list, account_node_t);
    paging.offset = 0;
    paging.pageSize = ACCOUNT_PAGE_SIZE;
    paging.totalRecords = 0;

    paging.totalRecords = Account_Srv_FetchAll(list);
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
        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice)
        {
        case 'a':
        case 'A':
            system(CLEAR);
            system(CLEAR);
            if (Account_UI_Add(list))
            {
                paging.totalRecords += 1;
                Paging_Locate_LastPage(list, paging, account_node_t);
            }
            break;
        case 'd':
        case 'D':
            system(CLEAR);
            printf("Input the user name you want to delete: ");
            scanf("%s", name);
            if (Account_UI_Delete(list, name))
            {
                paging.totalRecords = Account_Srv_FetchAll(list);
                List_Paging(list, paging, account_node_t);
            }
            break;
        case 'm':
        case 'M':
            system(CLEAR);
            printf("Input the user name you want to modify: ");
            scanf("%s", name);
            if (Account_UI_Modify(list, name))
            {
                paging.totalRecords = Account_Srv_FetchAll(list);
                List_Paging(list, paging, account_node_t);
            }
            break;
        case 'q':
        case 'Q':
            system(CLEAR);
            printf("Input the user name you want to query: ");
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