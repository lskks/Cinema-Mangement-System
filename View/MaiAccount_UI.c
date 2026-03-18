#include <stdio.h>
#include <string.h>
#include "MaiAccount_UI.h"

void MaiAccount_UI_MgtEntry(void)
{
    account_t cdata = gl_CurUser;
    char cpwd[30];

    printf("Pleas enter the new password for user %s: ", cdata.username);
    scanf("%s", cpwd);
    strcpy(cdata.password, cpwd);

    Account_Srv_Modify(&data);

    printf("This is the account management entry.\n");
}