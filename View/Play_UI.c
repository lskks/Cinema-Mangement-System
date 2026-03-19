#include "../View/Play_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/Seat.h"


static const int PLAY_PAGE_SIZE = 5;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void Play_UI_MgtEntry(void) {
    int choice;
    play_list_t list = NULL;
    Play_Srv_FetchAll(list); 
    do {
        system("cls"); 
        printf("========== Play Management ==========\n");
        printf("1. Add Play\n");
        printf("2. Modify Play\n");
        printf("3. Delete Play\n");
        printf("4. Query Play\n");
        printf("5. Schedule Performance\n");
        printf("0. Return to Main Menu\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: Play_UI_Add(); break;
        case 2: {
            int id;
            printf("Please enter the ID of the play to modify: ");
            scanf("%d", &id);
            Play_UI_Modify(id);
            break;
        }
        case 3: {
            int id;
            printf("Please enter the ID of the play to delete: ");
            scanf("%d", &id);
            Play_UI_Delete(id);
            break;
        }
        case 4: {
            int id;
            printf("Please enter the ID of the play to query: ");
            scanf("%d", &id);
            Play_UI_Query(id);
            break;
        }
        case 5: 
            //Schedule_UI_MgtEntry();
            break;
        case 0: 
            printf("Returning to main menu...\n");
            break;
        default: printf("Invalid input! Please try again.\n");
        }
        system("pause"); 
    } while (choice != 0);
}

int Play_UI_Add(void) {
    play_t data;
    memset(&data, 0, sizeof(play_t));

    printf("========== Add New Play ==========\n");
    printf("Please enter play name: ");
    scanf("%s", data.name);
    printf("Please enter play type (1-Drama 2-Opera 3-Concert): ");
    scanf("%d", (int*)&data.type);
    printf("Please enter play area: ");
    scanf("%s", data.area);
    printf("Please enter play rating (1-Child 2-Teenage 3-Adult): ");
    scanf("%d", (int*)&data.rating);
    printf("Please enter play duration (minutes): ");
    scanf("%d", &data.duration);
    printf("Please enter start date (Year Month Day): ");
    scanf("%d %d %d", &data.start_date.year, &data.start_date.month, &data.start_date.day);
    printf("Please enter end date (Year Month Day): ");
    scanf("%d %d %d", &data.end_date.year, &data.end_date.month, &data.end_date.day);
    printf("Please enter play price: ");
    scanf("%d", &data.price);

    int tmp = Play_Srv_Add(&data);
    if (tmp > 0) {
        printf("Added successfully! New play ID: %d\n", data.id);
    }
    else {
        printf("Failed to add play!\n");
    }
    return tmp;
}

int Play_UI_Modify(int id) {
    play_t buf;
    if (Play_Srv_FetchByID(id, &buf) == 0) {
        printf("No play found with ID: %d!\n", id);
        return 0;
    }

    printf("========== Modify Play (ID:%d) ==========\n", id);
    printf("Current name: %s ¡ú New name: ", buf.name);
    scanf("%s", buf.name);
    printf("Current type (1-Drama 2-Opera 3-Concert): %d ¡ú New type: ", buf.type);
    scanf("%d", (int*)&buf.type);
    printf("Current area: %s ¡ú New area: ", buf.area);
    scanf("%s", buf.area);
    printf("Current rating (1-Child 2-Teenage 3-Adult): %d ¡ú New rating: ", buf.rating);
    scanf("%d", (int*)&buf.rating);
    printf("Current duration: %d ¡ú New duration (minutes): ", buf.duration);
    scanf("%d", &buf.duration);
    printf("Current start date: %d-%d-%d ¡ú New start date (Year Month Day): ",
        buf.start_date.year, buf.start_date.month, buf.start_date.day);
    scanf("%d %d %d", &buf.start_date.year, &buf.start_date.month, &buf.start_date.day);
    printf("Current end date: %d-%d-%d ¡ú New end date (Year Month Day): ",
        buf.end_date.year, buf.end_date.month, buf.end_date.day);
    scanf("%d %d %d", &buf.end_date.year, &buf.end_date.month, &buf.end_date.day);
    printf("Current price: %d ¡ú New price: ", buf.price);
    scanf("%d", &buf.price);

    int tmp = Play_Srv_Modify(&buf);
    if (tmp > 0) {
        printf("Modified successfully!\n");
    }
    else {
        printf("Failed to modify play!\n");
    }
    return tmp;
}

int Play_UI_Delete(int id) {
    char confirm;
    printf("Are you sure to delete play with ID %d? (y/n): ", id);
    scanf(" %c", &confirm); 

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled!\n");
        return 0;
    }

    int tmp = Play_Srv_DeleteByID(id);
    if (tmp > 0) {
        printf("Deleted successfully!\n");
    }
    else {
        printf("Failed to delete (play may not exist or has related performances)!\n");
    }
    return tmp;
}

int Play_UI_Query(int id) {
    play_t date;
    if (Play_Srv_FetchByID(id, &date) == 0) {
        printf("No play found with ID: %d!\n", id);
        return 0;
    }

    printf("========== Play Details (ID:%d) ==========\n", id);
    printf("| ID   | Name                         | Type     | Area     | Rating | Duration | Performance Date       | Price   |\n");
    play_t* p = &date;
    printf("| %-4d | %-30s | %-8s | %-9s | %-6d | %d-%d-%d ~ %d-%d-%d | %-6d |\n",
        p->id, p->name,
        (p->type == PLAY_TYPE_FILE ? "Drama" : (p->type == PLAY_TYPE_OPEAR ? "Opera" : "Concert")),
        (p->rating == PLAY_RATE_CHILD ? "Child" : (p->rating == PLAY_RATE_TEENAGE ? "Teenage" : "Adult")),
        p->duration,
        p->start_date.year, p->start_date.month, p->start_date.day,
        p->end_date.year, p->end_date.month, p->end_date.day,
        p->price);
    return 1;
}