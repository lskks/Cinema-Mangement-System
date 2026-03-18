#include "Account_Persist.h"
#include "../Common/List.h"
#include "EntityKey_Persist.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ACCOUNT_KEY_NAME "account"

int Account_Perst_CheckAccFile()
{
    return access(ACCOUNT_DATA_FILE, F_OK) == 0;
}

int Account_Perst_Insert(account_t *data)
{
    size_t rtn = 0;
    int key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1);

    if (key <= 0)
        return 0;

    FILE *file = fopen(ACCOUNT_DATA_FILE, "ab");

    data->id = key;

    if (file == NULL)
    {
        fprintf(stderr, "open file %s failed", ACCOUNT_DATA_FILE);
        return rtn;
    }
    rtn = fwrite(data, sizeof(account_t), 1, file);
    fclose(file);
    return rtn;
}

int Account_Perst_Update(account_t *data)
{
    assert(NULL != data);

    FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+"); // 以读写方式打开二进制文件
    if (NULL == fp)
    {
        fprintf(STDERR_FILENO, "Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    account_t buf;
    int found = 0;

    while (!feof(fp))
    {
        if (fread(&buf, sizeof(account_t), 1, fp))
        {
            if (buf.id == data->id)
            {

                fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
                fwrite(data, sizeof(account_t), 1, fp);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found;
}

int Account_Perst_RemById(int id)
{
    if (rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE) < 0)
    {
        fprintf(stderr, "cannot open file %s", ACCOUNT_DATA_FILE);
        return 0;
    }

    FILE *fpOld = fopen(ACCOUNT_DATA_FILE, "rb");
    FILE *fpNew = fopen(ACCOUNT_DATA_TEMP_FILE, "wb");

    if (fpOld == NULL || fpNew == NULL)
    {
        if (fpOld)
            fclose(fpOld);
        if (fpOld)
            fclose(fpOld);
        return 0;
    }

    account_t buf;
    int found = 0;

    while (fread(&buf, sizeof(account_t), 1, fpOld) == 1)
    {
        if (buf.id == id)
            found = 1;
        else
            fwrite(&buf, sizeof(account_t), 1, fpNew);
    }

    fclose(fpOld);
    fclose(fpNew);

    remove(ACCOUNT_DATA_TEMP_FILE);
    return found;
}

int Account_Perst_SelectAll(account_list_t list)
{
    account_node_t *newNode;
    account_t data;
    int recCount = 0;

    assert(NULL != list);

    List_Free(list, account_node_t);

    FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (NULL == fp)
        return 0;

    while (!feof(fp))
    {
        if (fread(&data, sizeof(account_t), 1, fp))
        {
            newNode = (account_node_t *)malloc(sizeof(account_node_t));
            if (!newNode)
            {
                printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
                break;
            }
            newNode->data = data;
            List_AddTail(list, newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}

/**
 * @brief 
 *      Find the account by name
 * @param username 
 * @param buf 
 * @return int if found return 1, else return 0
 */
int Account_Perst_SelectByName(char *username, account_t *buf)
{
    if (NULL == username || NULL == buf)
    {
        return 0;
    }

    if (strlen(username) > 30)
    {
        return 0;
    }

    FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (NULL == fp)
    {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    account_t data;
    int found = 0;

    while (fread(&data, sizeof(account_t), 1, fp) == 1)
    {
        if (strcmp(data.username, username) == 0)
        {
            memcpy(buf, &data, sizeof(account_t));
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}