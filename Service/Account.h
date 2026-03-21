#pragma once

#include <string.h>
#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#define ACCOUNT_PAGE_SIZE 5
#define ACCOUNT_NAME_LEN 30
#define ACCOUNT_PWD_LEN 30

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
    account_type_t  type;
    char username[ACCOUNT_NAME_LEN];
    char password[ACCOUNT_PWD_LEN];
} account_t;

/**
 * @var a global user to save system user information
 */
extern account_t gl_CurUser;

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

int Account_Srv_FetchByName(char* userName, account_t* data);

account_node_t* Account_Srv_FindByUserName(account_list_t list, char* userName);

static inline char* account_type2str(account_type_t type)
{
    switch (type)
    {
    case USR_ANOMY:
        return "Anonymous";
    case USR_CLERK:
        return "Clerk";
    case USR_MANG:
        return "Manager";
    case USR_ADMIN:
        return "Admin";
    default:
        return "Unknown";
    }
}

static inline account_type_t str2account_type(const char* str)
{
    if (strcmp(str, "Anonymous") == 0)
        return USR_ANOMY;
    else if (strcmp(str, "Clerk") == 0)
        return USR_CLERK;
    else if (strcmp(str, "Manager") == 0)
        return USR_MANG;
    else if (strcmp(str, "Admin") == 0)
        return USR_ADMIN;
    else
        return -1; // Invalid type
}

#endif
