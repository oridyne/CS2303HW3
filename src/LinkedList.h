/*
 * LinkedList.h
 *
 *  Created on: Nov 4, 2019
 *      Author: Therese
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include "Room.h"

typedef struct
{
	int roomNumber;
	float treasure;
} SearchResults;

struct RoomNode;

typedef struct
{
	struct RoomNode* next;
	struct RoomNode* prev;
	Room* roomP;
} RoomNode;

typedef struct
{
	struct SearchResultNode* next;
	struct SearchResultNode* prev;
	SearchResults* searchP;
} SearchResultNode;

typedef struct
{
	Room* mp;
	RoomNode* newQHead;
} backFromDQFIFO;

RoomNode* makeEmptyRoomLL();
SearchResultNode* makeEmptySearchLL();
RoomNode* removeFromList(RoomNode* hp, Room* pP);
void saveRoom(RoomNode* lp, Room* mp);
void saveSearchResult(SearchResultNode* lp, SearchResults* mp);
bool isRoomLLEmpty(RoomNode* lp);
Room* dequeueLIFO(RoomNode* lp);
backFromDQFIFO* dequeueFIFO(RoomNode* lp);
void printSearchHistory(SearchResultNode* hp);
void deleteRoomLL(RoomNode* node);
void deleteSearchLL(SearchResultNode* node);
#endif /* LINKEDLIST_H_ */
