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
static void clear_input()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
static int get_last_day(int year, int month)
{
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}
void StaSales_UI_MgtEntry(void)
{
    char choice;
    while (1)
    {
        system("clear");
        printf("========================================\n");
        printf("          统计销售额界面\n");
        printf("========================================\n\n");
        if (gl_CurUser.type == USR_CLERK)
        {
            printf("当前用户：%s (售票员)\n", gl_CurUser.username);
            printf("\n请选择操作:\n");
            printf("  S - 统计个人销售额\n");
            printf("  Q - 返回\n");
            printf("请输入选项: ");
            scanf("%c", &choice);
            clear_input();
            switch (choice)
            {
            case 'S':
            case 's':
                StaSales_UI_Self();
                break;
            case 'Q':
            case 'q':
                return;
            default:
                printf("无效选项!\n");
                break;
            }
        }
        else if (gl_CurUser.type == USR_MANG)
        {
            printf("当前用户：%s (经理)\n", gl_CurUser.username);
            printf("\n请选择操作:\n");
            printf("  C - 统计售票员销售额\n");
            printf("  Q - 返回\n");
            printf("请输入选项: ");
            scanf("%c", &choice);
            clear_input();
            switch (choice)
            {
            case 'C':
            case 'c':
                StaSales_UI_Clerk();
                break;
            case 'Q':
            case 'q':
                return;
            default:
                printf("无效选项！\n");
                break;
            }
        }
        else
        {
            printf("账号没有权限！\n");
            printf("按回车键返回...");
            getchar();
            return;
        }

        printf("\n按回车键继续...");
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
    printf("\n========================================\n");
    printf("          统计个人销售额\n");
    printf("========================================\n");
    printf("售票员：%s (ID: %d)\n", gl_CurUser.username, id);
    printf("当前日期：%04d-%02d-%02d\n", curdate.year, curdate.month, curdate.day);
    printf("\n请选择统计类型:\n");
    printf("  D - 当日销售额\n");
    printf("  M - 当月销售额\n");
    printf("  Q - 返回\n");

    printf("请输入选项: ");
    scanf("%c", &choice);
    clear_input();

    switch (choice)
    {
    case 'D':
    case 'd':
        amount = StaSales_Srv_CompSaleVal(id, curdate, curdate);
        printf("\n--- 统计结果 ---\n");
        printf("售票员：%s\n", gl_CurUser.username);
        printf("统计日期：%04d-%02d-%02d\n", curdate.year, curdate.month, curdate.day);
        printf("当日销售额：%d 元\n", amount);
        break;

    case 'M':
    case 'm':
        amount = StaSales_Srv_CompSaleVal(id, startdate, enddate);
        printf("\n--- 统计结果 ---\n");
        printf("售票员：%s\n", gl_CurUser.username);
        printf("统计月份：%04d-%02d\n", startdate.year, startdate.month);
        printf("当月销售额：%d 元\n", amount);
        break;

    case 'Q':
    case 'q':
        return;

    default:
        printf("无效选项！\n");
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
    printf("\n========================================\n");
    printf("          统计售票员销售额\n");
    printf("========================================\n");
    printf("请输入售票员姓名: ");
    fgets(Usrname, sizeof(Usrname), stdin);
    Usrname[strcspn(Usrname, "\n")] = 0;
    if (!Account_Srv_FetchByName(Usrname, &account))
    {
        printf("用户 \"%s\" 不存在！\n", Usrname);
        return;
    }

    id = account.id;
    printf("找到用户：%s (ID: %d, 类型: %s)\n", account.username, account.id,
           account.type == USR_CLERK ? "售票员" : "经理");

    if (account.type != USR_CLERK)
    {
        printf("错误：只能统计售票员的销售额！\n");
        return;
    }

    printf("\n请输入开始日期(格式: yyyy mm dd): ");
    scanf("%d %d %d", &startdate.year, &startdate.month, &startdate.day);
    printf("请输入结束日期(格式: yyyy mm dd): ");
    scanf("%d %d %d", &enddate.year, &enddate.month, &enddate.day);
    clear_input();

    amount = StaSales_Srv_CompSaleVal(id, startdate, enddate);

    printf("\n--- 统计结果 ---\n");
    printf("售票员：%s (ID: %d)\n", account.username, id);
    printf("统计区间：%04d-%02d-%02d 至 %04d-%02d-%02d\n", startdate.year, startdate.month,
           startdate.day, enddate.year, enddate.month, enddate.day);
    printf("销售额：%d 元\n", amount);
}
