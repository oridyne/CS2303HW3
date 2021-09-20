/*
 * LinkedList.c
 *
 *  Created on: Nov 4, 2019
 *      Author: Therese
 */


#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>

bool isRoomLLEmpty(RoomNode* lp)
{
	bool ans = false;
	if(lp->roomP == (Room*)0) {
		ans = true;
	}
	return ans;
}
bool isSearchLLEmpty(SearchResultNode* lp)
{
	bool ans = false;
	if(lp->searchP == (SearchResults*)0) {
		ans = true;
	}
	return ans;
}

RoomNode* makeEmptyRoomLL()
{
	RoomNode* lp = (RoomNode*) malloc(sizeof(RoomNode));
	lp->next = (struct RoomNode*)0;
	lp->prev = (struct RoomNode*)0;
	lp->roomP = (Room*)0;

	return lp;
}
SearchResultNode* makeEmptySearchLL()
{
	SearchResultNode* lp = (SearchResultNode*) malloc(sizeof(SearchResultNode));
	lp->next = (struct SearchResultNode*)0;
	lp->prev = (struct SearchResultNode*)0;
	lp->searchP = (SearchResults*)0;

	return lp;
}
void saveRoom(RoomNode* lp, Room* mp)
{
	//if the list is empty, then make payP be mp
	//else traverse the list,
	//make a new list element
	//put mp in that
	//attach the new list element to the existing list
	if(isRoomLLEmpty(lp))
	{
		lp->roomP = mp;
	}
	else
	{
		RoomNode* temp = lp;
		while(temp->next)
		{
			temp=(RoomNode*)temp->next;
		}
		//now temp points to the last element

		//make a new element, attach mp to it, wire up the new element
		RoomNode* newList = makeEmptyRoomLL();
		newList->roomP = mp;
		temp->next = (struct RoomNode*)newList;
		newList->prev = (struct RoomNode*) temp;
	}
}
void saveSearchResult(SearchResultNode* lp, SearchResults* mp)
{
	//if the list is empty, then make payP be mp
	//else traverse the list,
	//make a new list element
	//put mp in that
	//attach the new list element to the existing list

	if(isSearchLLEmpty(lp))
	{
		lp->searchP = mp;
	}
	else
	{
		SearchResultNode* temp = lp;
		while(temp->next)
		{
			temp=(SearchResultNode*)temp->next;
		}
		//now temp points to the last element

		//make a new element, attach mp to it, wire up the new element
		SearchResultNode* newList = makeEmptySearchLL();
		newList->searchP = mp;
		temp->next = (struct SearchResultNode*) newList;
		newList->prev = (struct SearchResultNode*) temp;
	}
}

Room* dequeueLIFO(RoomNode* lp)
{
	Room* roomP = (Room*)0;
	if(isRoomLLEmpty(lp))
	{
		puts("Trying to dequeue from empty.");
	}
	else
	{

		RoomNode* temp = lp;
		while(temp->next)
		{
			temp=(RoomNode*)temp->next;
		}
		//now temp points to the last element


		roomP = temp->roomP;
		temp->roomP = (Room*)0;
		printf("Room being retured is %d\n", roomP->roomNumber); fflush(stdout);
		//remove the last, now empty, element
		if(temp->prev)
		{
			temp=(RoomNode*)temp->prev;
			free(temp->next);
			printf("end of queue is room %d\n", temp->roomP->roomNumber);fflush(stdout);
			temp->next = (struct RoomNode*)0;
		}
		else
		{
			puts("Queue is now empty");
		}
		puts("returning from dequeue");fflush(stdout);
	}

	return roomP;
}

backFromDQFIFO* dequeueFIFO(RoomNode* lp)
{
	backFromDQFIFO* fp = (backFromDQFIFO*) malloc(sizeof(backFromDQFIFO));
	if(lp->next == (struct RoomNode*)0)
	{
		//list of length 1 or 0
		fp->newQHead= lp;
	}
	else
	{
		fp->newQHead= (RoomNode*) lp->next;
	}
	fp->mp= lp->roomP;//all return values are set
	if(lp->next != (struct RoomNode*)0)
	{
		//length list is >1
		free(lp);
	}

	return fp;
}

void printSearchHistory(SearchResultNode* hp)
{
    puts("Printing history");
    if(hp->searchP ==(SearchResults*)0)
    {
         puts("Empty list");
    }
    else
    {
	    //traverse the list, printing as we go
        float treasureSubtotal = 0.0f;
        int room = -1;
        SearchResultNode* temp = hp;
        while(temp->next)
        {
            room =temp->searchP->roomNumber;
            treasureSubtotal += temp->searchP->treasure;
            printf("The room was %d, and the treasure subtotal was %f.\n", room, treasureSubtotal);
            temp=(SearchResultNode*)temp->next;

        }
        room =temp->searchP->roomNumber;
        treasureSubtotal+= temp->searchP->treasure;
        printf("The room was %d, and the treasure subtotal was %f.\n", room, treasureSubtotal);
    }
}

RoomNode* removeFromList(RoomNode* hP, Room* pP) {
    RoomNode* retHead = hP; //only changes if first element gets removed
    if(!isRoomLLEmpty(hP)) {
        RoomNode* altHead = (RoomNode*)hP->next;
        RoomNode* temp = hP;
        bool done = false;
        while((!done) && temp->next) {
            if(temp->roomP == pP) {
                done=true;
            } else {
                temp=(RoomNode*)temp->next;
            }
        }
        if((temp->roomP) == pP) {
            if(temp == hP) {
                if(!(temp->next)) {
                    free(hP->roomP);
                    hP->roomP=NULL;
                } else {
                    // free both for no mem leaks
                    free(temp->roomP);
                    free(temp);
                    altHead->prev=NULL;
                    retHead = altHead;
                }
            } else {
                RoomNode* prevPart = (RoomNode*) temp->prev;
                RoomNode* nextPart = (RoomNode*) temp->next;
                prevPart->next = (struct RoomNode*) nextPart;
                if((bool)nextPart) {
                    nextPart->prev = (struct RoomNode*) prevPart;
                }
                // free both card and the node ptr
                free(temp->roomP);
                free(temp);
            }
        }
    }
    return retHead;
}

// No memory leaks
void deleteRoomLL(RoomNode* node) {
    RoomNode* tmp;
    while (node != NULL) {
        tmp = node;
        node = (RoomNode *) node->next;
        if(tmp->roomP != NULL) {
            free(tmp->roomP);
            tmp->roomP = NULL;
        }
        free(tmp);
    }
}

void deleteSearchLL(SearchResultNode* node) {
    SearchResultNode* tmp;
    while (node != NULL) {
        tmp = node;
        node = (SearchResultNode *) node->next;
        if(tmp->searchP != NULL) {
            free(tmp->searchP);
        }
        free(tmp);
    }
}
