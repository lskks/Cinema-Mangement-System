#ifndef PLAY_H_
#define PLAY_H_

typedef enum {
	PLAY_TYPE_FILE = 1,			
	PLAY_TYPE_OPEAR = 2,			
	PLAY_TYPE_CONCERT = 3			
}play_type_t;  // 剧目类型

typedef enum {
	PLAY_RATE_CHILD = 1,			
	PLAY_RATE_TEENAGE = 2,			
	PLAY_RATE_ADULT = 3			
}play_rating_t;  //剧目等级

typedef struct {
	int year;
	int month;
	int day;
} ttms_date_t;

typedef struct {
	int id;			
	char name[31];	
	play_type_t type;	
	char area[9];	
	play_rating_t rating;
	int duration;           //剧目时长，分钟为单位
	ttms_date_t start_date;
	ttms_date_t end_date;
	int price;
} play_t;

typedef struct play_node {
	play_t data;				//实体数据
	struct play_node* next;	//后向指针
	struct play_node* prev;	//前向指针
} play_node_t, *play_list_t;

int Play_Srv_Add(play_t* data);

int Play_Srv_Modify(const play_t* data);

int Play_Srv_DeleteByID(int ID);

int Play_Srv_FetchByID(int ID, play_t* buf);

int Play_Srv_FetchAll(play_list_t list);

play_node_t* Play_Srv_FindByID(play_list_t list, int ID);

#endif