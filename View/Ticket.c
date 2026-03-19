#include "Ticket.h"
#include "../Service/Ticket.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
static void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void Ticket_UI_MgtEntry(int schedule_id) {
    schedule_t schedule;
    play_t play;
    char choice;
    int result;
    int rtn;

    while (1) {
        system("clear"); // Linux: "clear", Windows: "cls"

        printf("========================================\n");
        printf("          生成演出票管理界面\n");
        printf("========================================\n\n");

        // a) 调用根据ID获取演出计划函数
        rtn = Schedule_Srv_FetchByID(schedule_id, &schedule);
        if (rtn != 1) {
            printf("演出计划不存在！(ID: %d)\n", schedule_id);
            printf("按回车键返回...");
            getchar();
            return;
        }

        // b) 使用剧目ID获取剧目信息
        rtn = Play_Srv_FetchByID(schedule.play_id, &play);

        // c) 在界面中显示信息
        printf("--- 演出计划信息 ---\n");
        printf("计划ID: %d\n", schedule.id);
        if (rtn == 1) {
            printf("剧目名称: %s\n", play.name);
            printf("剧目时长: %d分钟\n", play.duration);
            printf("基础票价: %d元\n", play.price);
        }
        printf("演出厅编号: %d\n", schedule.studio_id);
        printf("演出日期: %s\n", schedule.date);
        printf("演出时间: %s\n\n", schedule.time);

        // d) 显示功能菜单并接收用户输入
        printf("请选择操作:\n");
        printf("  G - 生成演出票\n");
        printf("  R - 重新生成演出票\n");
        printf("  Q - 返回上一级\n");
        printf("请输入选项: ");
        scanf("%c", &choice);
        clear_input();

        switch (choice) {
            case 'G':
            case 'g':
                // 调用业务逻辑层批量生成演出票
                result = Ticket_Srv_GenBatch(schedule_id);
                if (result >= 0) {
                    printf("演出票生成成功！共生成 %d 张票。\n", result);
                } else {
                    printf("演出票生成失败！\n");
                }
                break;

            case 'R':
            case 'r':
                // 先删除再生成
                result = Ticket_Srv_DeleteBatch(schedule_id);
                if (result >= 0) {
                    printf("已删除 %d 张原有演出票。\n", result);
                    result = Ticket_Srv_GenBatch(schedule_id);
                    if (result >= 0) {
                        printf("演出票重新生成成功！共生成 %d 张票。\n", result);
                    } else {
                        printf("重新生成失败！\n");
                    }
                } else {
                    printf("删除原有演出票失败！\n");
                }
                break;

            case 'Q':
            case 'q':
                printf("返回上一级...\n");
                return;

            default:
                printf("无效选项！请重新选择。\n");
        }

        // e) 等待用户确认后继续
        printf("\n按回车键继续...");
        getchar();
    }
}