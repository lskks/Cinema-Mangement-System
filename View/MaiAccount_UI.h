#ifndef MAIACCOUNT_UI_H_
#define MAIACCOUNT_UI_H_

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
    char username[30];
    char password[30];
} account_t;

/**
 *@var a global user to save system user information
 */
account_t gl_CurUser = {0, USR_ANOMY, "Anonymous", ""};

typedef struct account_node
{
    account_t data;
    struct account_node* next;
    struct account_node* prev;
} account_node_t, *account_list_t;

void MaiAccount_UI_MgtEntry(void);

#endif
