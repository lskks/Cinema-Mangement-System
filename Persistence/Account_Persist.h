#ifndef _ACCOUNT_PERSIST_H_
#define _ACCOUNT_PERSIST_H_

#include "../Service/Account.h"

#define ACCOUNT_DATA_FILE "account.dat"
#define ACCOUNT_DATA_TEMP_FILE "account.tmp"

int Account_Perst_CheckAccFile();

int Account_Perst_MigrateLegacyData(void);

int Account_Perst_Insert(account_t* data);

int Account_Perst_Update(account_t* data);

int Account_Perst_RemById(int id);

int Account_Perst_SelectAll(account_list_t list);

int Account_Perst_SelectByName(char* username, account_t* acc);

#endif