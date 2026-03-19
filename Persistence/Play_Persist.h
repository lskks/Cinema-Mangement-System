#ifndef PLAY_PERSIST_H_
#define PLAY_PERSIST_H_
#include "../Service/Play.h"

int Play_Perst_Insert(play_t* data);

int Play_Perst_Update(const play_t* data);

int Play_Perst_RemByID(int ID);

int Play_Perst_SelectByID(int ID, play_t* buf);

int Play_Perst_SelectAll(play_list_t list);

#endif