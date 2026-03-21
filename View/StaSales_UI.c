//
// Created by shizihao on 2026/3/20.
//
#include "../Common/common.h"
#include "../Service/Account.h"
#include "../Service/StaSales.h"
#include "StaSales_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int get_last_day(int year, int month)
{
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        else
            return 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else
        return 31;
}
void StaSales_UI_MgtEntry(void)
{
    char choice;
    while (1)
    {
        system(CLEAR);
        printf("\n==================================================================\n");
        printf("************************ StaSales Analysis ************************\n");
        printf("Current User: %s\n", gl_CurUser.username);

        if (gl_CurUser.type == USR_CLERK)
        {
            printf("Role: Clerk\n");
            printf("------------------------------------------------------------------\n");
            printf("[S]elf Sales Statistics\n");
            printf("[R]eturn\n");
            printf("==================================================================\n");
            printf("Please input your choice:");
            scanf(" %c", &choice);
            clear_input_buffer();
            switch (choice)
            {
            case 'S':
            case 's':
                StaSales_UI_Self();
                break;
            case 'R':
            case 'r':
                return;
            default:
                printf("Invalid choice!\n");
                break;
            }
        }
        else if (gl_CurUser.type == USR_MANG)
        {
            printf("Role: Manager\n");
            printf("------------------------------------------------------------------\n");
            printf("[C]lerk Sales Statistics\n");
            printf("[R]eturn\n");
            printf("==================================================================\n");
            printf("Please input your choice:");
            scanf(" %c", &choice);
            clear_input_buffer();
            switch (choice)
            {
            case 'C':
            case 'c':
                StaSales_UI_Clerk();
                break;
            case 'R':
            case 'r':
                return;
            default:
                printf("Invalid choice!\n");
                break;
            }
        }
        else
        {
            printf("Permission denied for current account.\n");
            printf("Press [Enter] key to return...");
            getchar();
            return;
        }

        printf("\nPress [Enter] key to continue...");
        getchar();
    }
}

void StaSales_UI_Self(void)
{
    int id = gl_CurUser.id;
    user_date_t curdate, startdate, enddate;
    time_t now;
    struct tm *timeNow;
    char choice;
    int amount;

    time(&now);
    timeNow = localtime(&now);

    curdate.year = timeNow->tm_year + 1900;
    curdate.month = timeNow->tm_mon + 1;
    curdate.day = timeNow->tm_mday;

    startdate.year = curdate.year;
    startdate.month = curdate.month;
    startdate.day = 1;

    enddate.year = curdate.year;
    enddate.month = curdate.month;
    enddate.day = get_last_day(curdate.year, curdate.month);
    system(CLEAR);
    printf("\n==================================================================\n");
    printf("********************** Self Sales Statistics **********************\n");
    printf("Clerk: %s (ID: %d)\n", gl_CurUser.username, id);
    printf("Current Date: %04d-%02d-%02d\n", curdate.year, curdate.month, curdate.day);
    printf("------------------------------------------------------------------\n");
    printf("[D]aily Sales\n");
    printf("[M]onthly Sales\n");
    printf("[R]eturn\n");
    printf("==================================================================\n");
    printf("Please input your choice:");
    scanf(" %c", &choice);
    clear_input_buffer();

    switch (choice)
    {
    case 'D':
    case 'd':
        amount = StaSales_Srv_CompSaleVal(id, curdate, curdate);
        printf("\n--- Statistics Result ---\n");
        printf("Clerk: %s\n", gl_CurUser.username);
        printf("Date: %04d-%02d-%02d\n", curdate.year, curdate.month, curdate.day);
        printf("Daily Sales: %d\n", amount);
        break;

    case 'M':
    case 'm':
        amount = StaSales_Srv_CompSaleVal(id, startdate, enddate);
        printf("\n--- Statistics Result ---\n");
        printf("Clerk: %s\n", gl_CurUser.username);
        printf("Month: %04d-%02d\n", startdate.year, startdate.month);
        printf("Monthly Sales: %d\n", amount);
        break;

    case 'R':
    case 'r':
        return;

    default:
        printf("Invalid choice!\n");
        break;
    }
}

void StaSales_UI_Clerk(void)
{
    int id;
    user_date_t startdate, enddate;
    char Usrname[30];
    account_t account;
    int amount;
    system(CLEAR);
    printf("\n==================================================================\n");
    printf("********************* Clerk Sales Statistics *********************\n");
    printf("Please input clerk username: ");
    scanf("%29s", Usrname);
    clear_input_buffer();
    if (!Account_Srv_FetchByName(Usrname, &account))
    {
        printf("User \"%s\" does not exist!\n", Usrname);
        return;
    }

    id = account.id;
    printf("Found user: %s (ID: %d, Type: %s)\n", account.username, account.id,
           account.type == USR_CLERK ? "Clerk" : "Manager");

    if (account.type != USR_CLERK)
    {
        printf("Error: only clerk accounts are allowed here!\n");
        return;
    }

    printf("\nPlease input start date (yyyy mm dd): ");
    scanf("%d %d %d", &startdate.year, &startdate.month, &startdate.day);
    printf("Please input end date (yyyy mm dd): ");
    scanf("%d %d %d", &enddate.year, &enddate.month, &enddate.day);
    clear_input_buffer();

    amount = StaSales_Srv_CompSaleVal(id, startdate, enddate);

    printf("\n--- Statistics Result ---\n");
    printf("Clerk: %s (ID: %d)\n", account.username, id);
    printf("Range: %04d-%02d-%02d to %04d-%02d-%02d\n", startdate.year, startdate.month,
           startdate.day, enddate.year, enddate.month, enddate.day);
    printf("Sales Amount: %d\n", amount);
}
