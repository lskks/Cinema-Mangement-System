#include <string.h>

#include "Account.h"
#include "../Persistence/Account_Persist.h"
// #include "Account_Persist.h"
#include "../Common/List.h"

void Account_Srv_InitSys()
{
    if(!Account_Perst_CheckAccFile())
    {
        return;
    }
    account_t data_admin;
    data_admin.username = "admin";
    data_admin.id = 0;
    data_admin.type = USR_ADMIN;
    data_admin.password = "123456";
    Account_Srv_Add(&data_admin);
}

int Account_Srv_Verify(char* username, char* pwd)
{
    account_t usr;
    if (!Account_Perst_SelectByName(username, &usr))
        return 0;

    if (strcmp(pwd, usr.password) != 0)
        return 0;

    gl_CurUser = usr;
    return 1;
}

int Account_Srv_Add(account_t* data)
{
    return Account_Perst_Insert(data);
}

int Account_Srv_Modify(account_t* data)
{
    return Account_Perst_Update(data);
}

int Account_Srv_DeleteByID(int userID)
{
    return Account_Perst_RemById(userID);
}

int Account_Srv_FetchAll(account_list_t list)
{
    return Account_Perst_SelectAll(list);
}

account_node_t* Account_Srv_FindByUserName(account_list_t list, char* userName)
{
    assert(NULL != list && NULL != userName);
    
    account_node_t *ptr;

    List_ForEach(list, ptr) {
        if (strcmp(ptr->data.username, userName) == 0) {
            return ptr;
        }
    }
    
    return NULL;
}
