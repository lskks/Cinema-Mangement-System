#include "Studio.h"
#include "../Persistence/Play_Persist.h"
#include <stdlib.h>
#include <assert.h>

int Play_Srv_Add(play_t* data) {
	return Play_Perst_Insert(data);
}

int Play_Srv_Modify(const play_t* data) {
	return Play_Perst_Update(data);
}

int Play_Srv_DeleteByID(int ID) {
	return Play_Perst_RemByID(ID);
}

int Play_Srv_FetchByID(int ID, play_t* buf) {
	return Play_Perst_SelectByID(ID, buf);

}

int Play_Srv_FetchAll(play_list_t list) {
	return Play_Perst_SelectAll(list);
}

play_node_t* Play_Srv_FindByID(play_list_t list, int ID) {
	assert(NULL != list);
	studio_node_t* ptr = list->next;
	while (ptr != list) {
		if (ptr->data.id == ID)
			return ptr;
		else
			ptr = ptr->next;
	}

	return NULL;
}
