#ifndef ACCOUNT_UI_H_H
#define ACCOUNT_UI_H_H

#include "../Service/Account.h"

int SysLogin();

void Account_UI_MgtEntry();

void Account_UI_Add(account_list_t list);

void Account_UI_Modify(account_list_t list, char username[]);

void Account_UI_Delete(account_list_t list, char username[]);

void Account_UI_Query(account_list_t list, char username[]);

#endif