/*
 * production.c
 *
 *  Created on: Jul 4, 2019
 *      Author: Therese
 */
#include "production.h"
#include "AdjMat.h"
#include "Room.h"
#include "LinkedList.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno ;

bool production(int argc, char* argv[])
{
	bool answer = false;

	if(argc <=1) //no interesting information
	{
		puts("Didn't find any arguments.");
		puts("Usage: HW3 filename maxRooms maxTreasure.");
		fflush(stdout);
		answer = false;
	}
	else if(argc != 4)
	{
		puts("Usage: HW3 filename maxRooms maxTreasure.");
	}
	else //there is interesting information
	{
		printf("Found %d interesting arguments.\n", argc-1);
		fflush(stdout);
		char filename[FILENAMELENGTHALLOWANCE];
		for (int i= 0; i< FILENAMELENGTHALLOWANCE; i++)
		{
			filename[i]=0;
		}
		//char* eptr=(char*) malloc(sizeof(char*));
		long aL=-1L;
		int maxRooms;
		float maxTreasure;
		double maxTreas;
		for(int i = 1; i<argc; i++) //don't want to read argv[0]
		{//argv[i] is a string
			//in this program our arguments are NR, NC, gens, filename, print and pause
			//because pause is optional, argc could be 6 or 7
			//because print is optional (if print is not present, neither is pause) argc could be 5
			switch(i)
			{
			case 1:
				//this is filename
				printf("The length of the filename is %d.\n",(int)strlen(argv[i]));
				printf("The proposed filename is %s.\n", argv[i]);
				if(strlen(argv[i])>=FILENAMELENGTHALLOWANCE)
				{
					puts("Filename is too long.");
					fflush(stdout);
					answer = false;
				}
				else
				{
					strcpy(filename, argv[i]);
					printf("Filename was %s.\n", filename);
					fflush(stdout);
				}
				break;
			case 2:
				//this is maximum number of rooms

				aL= strtol(argv[i], NULL, 10);
				maxRooms = (int) aL;
				printf("Number of rooms is %d\n",maxRooms);fflush(stdout);
				break;
			case 3:
				//this is maximum amount of treasure

				maxTreas = atof(argv[i]);
				printf("Amount of  treasure is %f\n",maxTreas);fflush(stdout);
				maxTreasure = (float) maxTreas;
				break;

			default:
				puts("Unexpected argument count."); fflush(stdout);
				answer = false;
				break;
			}//end of switch
		}//end of for loop on argument count
		puts("after reading arguments"); fflush(stdout);
		//we'll want to read the file
		int nrooms = -1;
		AdjMat* adjMP = (AdjMat*) malloc(sizeof(AdjMat));
		//cannot, do not know nrooms init(adjMP);
		//puts("Back from init Adj Mat"); fflush(stdout);
		Room* theRoomPs[10];//addresses for 10 rooms

		puts("Before read file"); fflush(stdout);
		answer = readFile(filename, &nrooms, adjMP, theRoomPs); //read the file
		puts("Back from read file"); fflush(stdout);

		//we'll want to conduct the search
		//for the search we'll need an empty search history
		SearchResultNode* historyP = makeEmptySearchLL();
		//we'll need the Queue, into which we put rooms, and remove rooms
		RoomNode* searchQ = makeEmptyRoomLL();
		puts("starting search"); fflush(stdout);
		//we'll start searching with room 0
		bool done = false;
		int searchedRooms = 0;
		float foundTreasure = 0.0;
		Room* roomBeingSearchedP = theRoomPs[0];
		//we set its searched field to true, and take its treasure
		roomBeingSearchedP->searched = true;
		//we record it in the history
		SearchResults* srP = (SearchResults*) malloc(sizeof(SearchResults));
		srP->roomNumber= 0;
		srP->treasure = roomBeingSearchedP->treasure;
		if((srP->treasure <= maxTreas) && (maxRooms>0))
		{
			//we searched room 0, so it should be enqueued onto the history,
			//and not onto the "things-to-do" queue
            saveSearchResult(historyP, srP);//here we are enqueueing room 0
			puts("Enqueuing room 0");
			foundTreasure = roomBeingSearchedP->treasure;
			searchedRooms = 1;
		}

		while(!done)
		{   //here we want to find all of the rooms adjacent to the roomBeingSearched,
			//so we look in the adj matrix
			for(int col = 0; (col<nrooms)&&!done; col++)
			{
				//we check whether this room is neighboring
				printf("checking rooms %d and %d.\n", roomBeingSearchedP->roomNumber, col); fflush(stdout);
				if(getEdge(adjMP,roomBeingSearchedP->roomNumber, col)==1)
				{
					puts("found an edge"); fflush(stdout);
					//if so, we check whether room has been searched
					if(!(theRoomPs[col]->searched))
					{//if it hasn't been searched
						printf("Room %d hasn't already been searched.\n", col);
						//we set it to searched
						theRoomPs[col]->searched=true;
						if(((foundTreasure + theRoomPs[col]->treasure)<= maxTreasure)&&(searchedRooms < maxRooms))
							//we check whether we can take the treasure vs. limit
							//we check whether we've hit the room limit
						{//we enqueue it for search
							foundTreasure += theRoomPs[col]->treasure;
							searchedRooms++;
							printf("found treasure updated to %f.\n", foundTreasure);
							printf("enqueuing room %d.\n", col); fflush(stdout);
							printf("Before enqueuing queue empty reports %d\n", isRoomLLEmpty(searchQ));
                            saveRoom(searchQ, theRoomPs[col]);
							srP = (SearchResults*) malloc(sizeof(SearchResults));
							srP->roomNumber=theRoomPs[col]->roomNumber;
							srP->treasure = theRoomPs[col]->treasure;
                            saveSearchResult(historyP, srP);
							printf("After enqueuing, queue empty reports %d\n", isRoomLLEmpty(searchQ));
						}//check about search limits
					}//room can still be searched
				}//room is a neighbor

				if(foundTreasure >= maxTreasure)
				{
					done = true;
					puts("Done by treasure");
				}
				if (searchedRooms>=maxRooms)
				{
					done = true;
					puts("Done by rooms");
				}
			}//scan for all possible rooms to search from this room
			//time to get the next room
			if(isRoomLLEmpty(searchQ))
			{
				done=true;
				puts("Done by queue empty");
			}
			if(!done)
			{
				puts("Invoking  dequeue");fflush(stdout);
				roomBeingSearchedP = dequeueLIFO(searchQ);

			}
		}//while search is not done
		//search is now done, time to print the history
        printSearchHistory(historyP);

		// Clean up
        deleteSearchLL(historyP);
        deleteRoomLL(searchQ);
        for(int i = 0; i < adjMP->n; i++) {
            Room** aRoomP = theRoomPs;
            aRoomP = aRoomP + i;
            free(*aRoomP);
        }
        free(adjMP->edgesP);
        free(adjMP);
	}//end of else we have good arguments
	return answer;
}

bool readFile(char* filename, int* nrooms, AdjMat* adjMP, Room** theRoomPs)
{
	bool ok = true;
	//the file tells how many rooms there are
	FILE* fp = fopen(filename, "r"); //read the file
	if (fp == NULL)
	{
		int errnum = errno;
		fprintf(stderr, "Value of errno: %d\n", errno);
		perror("Error printed by perror");
		fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
		puts("Error! opening file");
		ok = false;
	}
	else
	{
		fscanf(fp,"%d", nrooms);
		adjMP->n= *nrooms;
		int howManyRooms = *nrooms;
		adjMP->edgesP = (int*) malloc(howManyRooms * howManyRooms *sizeof(int));
		puts("Before init Adj Mat"); fflush(stdout);
		init(adjMP);
		int temp = -1;
		for(int roomr = 1; roomr<*nrooms; roomr++)
		{
			printf("on row %d\n", roomr);fflush(stdout);
			for(int roomc = 0; roomc<roomr; roomc++)
			{
				fscanf(fp,"%d", &temp);
				printf("in column %d, read %d\n", roomc, temp);fflush(stdout);
				//now set the value in the adjacency matrix
				if(temp==1)
				{
					setEdge(adjMP, roomr, roomc);
				}

			}
		}

		float tempTreas = 2.9;
		for(int roomr = 0; roomr<*nrooms; roomr++)
		{
			fscanf(fp,"%f", &tempTreas);
			//make a room and set its treasure
			Room** aRoomP = theRoomPs;
			aRoomP = aRoomP+roomr;
			*aRoomP = (Room*) malloc(sizeof(Room));
			//now set the treasures
			(*aRoomP)->treasure = tempTreas;
			(*aRoomP)->roomNumber = roomr;
			(*aRoomP)->searched = false;
			printf("The treasure in room %d is %f\n", roomr, (*aRoomP)->treasure);
		}
		ok = true;
	}
	fclose(fp);

	//the file contains a triangular array of integers, that are either 1 or 0


	return ok;
}

struct Board** setUpBoards(int nPlayers)
{
	struct Board** bp = (struct Board**) 0;
	return bp;
}
bool getYesNo(char* query)
{
   bool answer = true; //so far
   char said = 'x';
   do
   {
	 printf("%s (y/n):",query);
     fflush(stdout);
     fflush(stdin);
     scanf("%c",&said);
   }while((said!= 'y')&&(said!='n'));
   if(said=='n')
   {
	   answer=false;
   }


   return answer;
}

