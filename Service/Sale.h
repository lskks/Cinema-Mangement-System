#ifndef Sale_H_
#define Sale_H_
#include "../Service/Play.h"


typedef struct {
    int hour;
    int minute;
    int second;
} ttms_time_t;

// 交易类型枚举
typedef enum {
    SALE_SELL = 1,    // 买票
    SALE_REFUND = -1  // 退票
} sale_type_t;

typedef enum {
    TICKET_AVL=0,
    TICKET_SOLD =1,
    TICKET_RESV =9
} ticket_status_t;

// 订单信息实体
typedef struct {
    long id;          
    int user_id;      
    int ticket_id;    
    ttms_date_t date; 
    ttms_time_t time; 
    int value;        
    sale_type_t type; 
} sale_t;

// 订单链表节点
typedef struct sale_node {
    sale_t data;          
    struct sale_node* next; 
    struct sale_node* prev; 
} sale_node_t, * sale_list_t;


typedef struct {
    int id;          
    ticket_status_t status;       
    int schedule_id;  
    int seat_id;      
    int price;
} ticket_t;

typedef struct {
    int id;     
    int play_id;
    int studio_id;
    ttms_date_t date;
   
} schedule_t;

typedef struct schedule_node {
    schedule_t data;          
    struct schedule_node* next; 
    struct schedule_node* prev; 
} schedule_node_t, *schedule_list_t;

typedef struct ticket_node {
    ticket_t data;
    struct ticket_node* next;
    struct ticket_node* prev;
} ticket_node_t, * ticket_list_t;

int Sale_Srv_Add(const sale_t* data);

int Ticket_Srv_Modify(const ticket_t* data);

int Ticket_Srv_FetchBySchID(int id, ticket_list_t* list);

ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id);

//other people work
//Schedule_Srv_FetchByPlay;
//Seat_Srv_FetchByRoom;
//Seat_Sre_FindByRC;

#endif