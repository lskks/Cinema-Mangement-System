/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: Main_Menu.c			 	 
* Description : TTMS 系统主菜单	
* Author:   XUPT  		 
* Version:  v.1 	 
* Da
*/


#include <stdio.h>
#include <stdlib.h>

#include "../View/Main_Menu.h"
#include "../View/Studio_UI.h"
#include "../View/Account_UI.h"
#include "../View/Sale_UI.h"
#include "../View/Play_UI.h"
#include "../View/Ticket_UI.h"
#include "../View/SalesAnalysis_UI.h"
#include "../Common/common.h"
#include "StaSales_UI.h"


void Queries_Menu()
{
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************** Queries Menu ****************\n");
		printf("[T]icket Query.\n");
		printf("[R]eturn.\n");
		printf("[S]taSales Analysis.\n");
		printf("\n==================================================================\n");
		printf("Please input your choice:");
		scanf(" %c", &choice);
		switch (choice)
		{
		case 'T':
		case 't':
			system(CLEAR);
			Ticket_UI_Query();
			break;
		case 'S':
		case 's':
			system(CLEAR);
			StaSales_UI_MgtEntry();
			break;
		}
		clear_input_buffer();
	}while (choice != 'r' && choice != 'R');
}

//系统主菜单 
void Main_Menu(void) {
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************** Theater Ticket Management System ****************\n");
		printf("[S]tudio Management.\n");
		printf("[P]lay Management.\n");
		printf("[T]icket Sale.\n");
		printf("[R]eturn Ticket.\n");
		printf("[Q]ueries\n");
		printf("Ra[n]king and Statistics.\n");
		printf("[A]ccount Management.\n");
		printf("[E]xist.\n");
		printf("\n==================================================================\n");
		printf("Please input your choice:");
		scanf("%c", &choice);
		clear_input_buffer();
		switch (choice) {
		case 'S':
		case 's':
			system(CLEAR);
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			system(CLEAR);
			Play_UI_MgtEntry();
			break;
		case 'Q':
		case 'q':
			system(CLEAR);
			Queries_Menu();
			break;
		case 'T':
		case 't':
			system(CLEAR);
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			system(CLEAR);
			Sale_UI_RefundTicket();
			break;
		case 'N':
		case 'n':
			system(CLEAR);
			if (SysLogin()) {
				SalesAnalysis_UI_MgtEntry();
			}
			break;
		case 'A':
		case 'a':
			system(CLEAR);
			if (SysLogin()) {
				Account_UI_MgtEntry();
			}
			break;
		}
	} while ('E' != choice && 'e' != choice);
}
