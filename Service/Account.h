#ifndef ACCOUNT_H_
#define ACCOUNT_H_

typedef enum
{
    USR_ANOMY = 0, // for initilization
    USR_CLERK = 1, // clerk
    USR_MANG = 2,  // Manger
    USR_ADMIN = 9,
} account_type_t;

typedef struct
{
    int id;
    account_type_t type;
    char* username;
    char* password;
} account_t;

/**
 * @var a global user to save system user information
 */
account_t gl_CurUser = {0, USR_ANOMY, "Anonymous", ""};

typedef struct account_node
{
    account_t data;
    struct account_node* next;
    struct account_node* prev;
} account_node_t, *account_list_t;

void Account_Srv_InitSys();

int Account_Srv_Verify(char* username, char* pwd);

int Account_Srv_Add(account_t* data);

int Account_Srv_Modify(account_t* data);

int Account_Srv_DeleteByID(int userID);

int Account_Srv_FetchAll(account_list_t list);

account_node_t* Account_Srv_FindByUserName(account_list_t list, char* userName);

#endif