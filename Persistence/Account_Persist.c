#include "Account_Persist.h"
#include "../Common/List.h"
#include "EntityKey_Persist.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ACCOUNT_KEY_NAME "account"

#define LEGACY_ACCOUNT_REC_SIZE 24

static int is_valid_account_type(int type)
{
    return type == USR_ANOMY || type == USR_CLERK || type == USR_MANG || type == USR_ADMIN;
}

static int is_valid_cstr_field(const char *buf, int len)
{
    int has_null = 0;
    for (int i = 0; i < len; i++)
    {
        unsigned char ch = (unsigned char)buf[i];
        if (ch == '\0')
        {
            has_null = 1;
            break;
        }
        if (!isprint(ch))
        {
            return 0;
        }
    }
    return has_null;
}

static int parse_new_account_record(const unsigned char *rec, account_t *out)
{
    account_t tmp;
    memcpy(&tmp, rec, sizeof(account_t));
    if (tmp.id <= 0 || !is_valid_account_type(tmp.type))
    {
        return 0;
    }
    if (!is_valid_cstr_field(tmp.username, ACCOUNT_NAME_LEN) ||
        !is_valid_cstr_field(tmp.password, ACCOUNT_PWD_LEN))
    {
        return 0;
    }
    *out = tmp;
    return 1;
}

static int parse_legacy_account_record(const unsigned char *rec, account_t *out)
{
    int id = 0;
    int type = 0;

    memcpy(&id, rec, sizeof(int));
    memcpy(&type, rec + sizeof(int), sizeof(int));

    if (id <= 0 || !is_valid_account_type(type))
    {
        return 0;
    }

    memset(out, 0, sizeof(account_t));
    out->id = id;
    out->type = (account_type_t)type;
    snprintf(out->username, ACCOUNT_NAME_LEN, "legacy_%d", id);
    snprintf(out->password, ACCOUNT_PWD_LEN, "legacy");
    return 1;
}

static int next_record_from_buffer(const unsigned char *buf, long len, long *offset, account_t *out)
{
    long pos = *offset;

    if (pos + (long)sizeof(account_t) <= len && parse_new_account_record(buf + pos, out))
    {
        *offset = pos + (long)sizeof(account_t);
        return 1;
    }

    if (pos + LEGACY_ACCOUNT_REC_SIZE <= len && parse_legacy_account_record(buf + pos, out))
    {
        *offset = pos + LEGACY_ACCOUNT_REC_SIZE;
        return 1;
    }

    return 0;
}

int Account_Perst_MigrateLegacyData(void)
{
    FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
    if (fp == NULL)
    {
        return 0;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    long file_len = ftell(fp);
    if (file_len <= 0)
    {
        fclose(fp);
        return 0;
    }
    rewind(fp);

    unsigned char *raw = (unsigned char *)malloc((size_t)file_len);
    if (raw == NULL)
    {
        fclose(fp);
        return 0;
    }

    if (fread(raw, 1, (size_t)file_len, fp) != (size_t)file_len)
    {
        free(raw);
        fclose(fp);
        return 0;
    }
    fclose(fp);

    if (file_len % (long)sizeof(account_t) == 0)
    {
        int all_valid = 1;
        for (long pos = 0; pos < file_len; pos += (long)sizeof(account_t))
        {
            account_t probe;
            if (!parse_new_account_record(raw + pos, &probe))
            {
                all_valid = 0;
                break;
            }
        }
        if (all_valid)
        {
            free(raw);
            return 0;
        }
    }

    char backup_file[] = ACCOUNT_DATA_FILE ".bak";
    remove(backup_file);
    if (rename(ACCOUNT_DATA_FILE, backup_file) != 0)
    {
        free(raw);
        return 0;
    }

    FILE *out = fopen(ACCOUNT_DATA_FILE, "wb");
    if (out == NULL)
    {
        rename(backup_file, ACCOUNT_DATA_FILE);
        free(raw);
        return 0;
    }

    int migrated = 0;
    long offset = 0;
    while (offset < file_len)
    {
        account_t rec;
        long prev = offset;
        if (next_record_from_buffer(raw, file_len, &offset, &rec))
        {
            rec.username[ACCOUNT_NAME_LEN - 1] = '\0';
            rec.password[ACCOUNT_PWD_LEN - 1] = '\0';
            fwrite(&rec, sizeof(account_t), 1, out);
            migrated++;
            continue;
        }

        if (prev + LEGACY_ACCOUNT_REC_SIZE <= file_len)
        {
            offset = prev + LEGACY_ACCOUNT_REC_SIZE;
        }
        else
        {
            break;
        }
    }

    fclose(out);
    free(raw);
    return migrated;
}

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
        fprintf(stderr, "Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    account_t buf;
    int found = 0;

    while (fread(&buf, sizeof(account_t), 1, fp) == 1)
    {
        if (buf.id == data->id)
        {
            fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
            fwrite(data, sizeof(account_t), 1, fp);
            found = 1;
            break;
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

    FILE *fpOld = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
    FILE *fpNew = fopen(ACCOUNT_DATA_FILE, "wb");

    if (fpOld == NULL || fpNew == NULL)
    {
        if (fpOld)
            fclose(fpOld);
        if (fpNew)
            fclose(fpNew);
        rename(ACCOUNT_DATA_TEMP_FILE, ACCOUNT_DATA_FILE);
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

    if (found)
    {
        remove(ACCOUNT_DATA_TEMP_FILE);
    }
    else
    {
        remove(ACCOUNT_DATA_FILE);
        rename(ACCOUNT_DATA_TEMP_FILE, ACCOUNT_DATA_FILE);
    }

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

    while (fread(&data, sizeof(account_t), 1, fp) == 1)
    {
        newNode = (account_node_t *)malloc(sizeof(account_node_t));
        if (!newNode)
        {
            printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
            break;
        }
        data.username[ACCOUNT_NAME_LEN - 1] = '\0';
        data.password[ACCOUNT_PWD_LEN - 1] = '\0';
        newNode->data = data;
        List_AddTail(list, newNode);
        recCount++;
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
        data.username[ACCOUNT_NAME_LEN - 1] = '\0';
        data.password[ACCOUNT_PWD_LEN - 1] = '\0';
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

int Account_Perst_SelectByID(int userID, account_t* data)
{
    if (NULL == data)
    {
        return 0;
    }

    FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+");
    if (NULL == fp)
    {
        printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
        return 0;
    }

    account_t buf;
    int found = 0;

    while (fread(&buf, sizeof(account_t), 1, fp) == 1)
    {
        buf.username[ACCOUNT_NAME_LEN - 1] = '\0';
        buf.password[ACCOUNT_PWD_LEN - 1] = '\0';
        if (buf.id == userID)
        {
            memcpy(data, &buf, sizeof(account_t));
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}