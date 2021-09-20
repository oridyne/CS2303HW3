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

	if(argc <= 1) //no interesting information
	{
		puts("Didn't find any arguments.");
		puts("Usage: HW3 filename maxRooms maxTreasure.");
		fflush(stdout);
		answer = false;
	}
	else //there is interesting information
	{
		printf("Found %d interesting arguments.\n", argc-1);
		fflush(stdout);
		char filename[FILENAMELENGTHALLOWANCE];
		for (int i= 0; i< FILENAMELENGTHALLOWANCE; i++) {
			filename[i]=0;
		}
		// char* eptr = (char*) malloc(sizeof(char*));
		long aL = -1L;
		int maxRooms = -1;
		float maxTreasure = -1;
		double maxTreas = 0.0;
		for(int i = 1; i < argc; i++) //don't want to read argv[0]
		{
		    //argv[i] is a string
			//in this program our arguments are NR, NC, gens, filename, print and pause
			//because pause is optional, argc could be 6 or 7
			//because print is optional (if print is not present, neither is pause) argc could be 5
			switch(i) {
			case 1:
				//this is filename
				printf("The length of the filename is %d.\n",(int)strlen(argv[i]));
				printf("The proposed filename is %s.\n", argv[i]);
				if(strlen(argv[i]) >= FILENAMELENGTHALLOWANCE) {
					puts("Filename is too long.");
					fflush(stdout);
					answer = false;
				} else {
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
				maxTreas = strtod(argv[i], NULL);
                maxTreasure = (float) maxTreas;
				printf("Amount of  treasure is %f\n",maxTreas);fflush(stdout);
				break;
			default:
				puts("Unexpected argument count."); fflush(stdout);
				answer = false;
				break;
			}//end of switch
		} //end of for loop on argument count
		puts("after reading arguments"); fflush(stdout);
		//we'll want to read the file
		int nrooms = -1;
		AdjMat* adjMP = (AdjMat*) malloc(sizeof(AdjMat));
		//cannot, do not know nrooms init(adjMP);
		puts("Back from init Adj Mat"); fflush(stdout);
		Room* theRoomPs[10]; //addresses for 10 rooms
		puts("Before read file"); fflush(stdout);
		answer = readFile(filename, &nrooms, adjMP, theRoomPs); //read the file
		puts("Back from read file"); fflush(stdout);

		// make sure read file is successful
        if(answer) {
            // get max treasure and max rooms if not supplied
            if(argc != 4) {
                getInputArgs(&maxRooms, &maxTreas, &maxTreasure);
            }
            float fileMaxTres = 0;
            for(int i = 0; i < nrooms; i++) {
                fileMaxTres += theRoomPs[i]->treasure;
            }
            if(maxRooms > nrooms) {
                printf("maxRooms (%d) is larger than maximum rooms from file (%d) \nSetting max rooms to %d\n",maxRooms,nrooms, nrooms);
                maxRooms = nrooms;
            }
            if(maxTreasure > fileMaxTres) {
                printf("maxTreasure (%f) is larger than maximum treasure from file (%f)\nSetting max treasure to %f\n",maxTreasure,fileMaxTres, fileMaxTres);
                maxTreasure = fileMaxTres;
            }
            //we'll want to conduct the search
            //for the search we'll need an empty search history
            SearchResultNode* historyP = makeEmptySearchLL();
            //we'll need the Queue, into which we put rooms, and remove rooms
            RoomNode* searchQ = makeEmptyRoomLL();
            puts("starting search"); fflush(stdout);
            //we'll start searching with room 0
            bool done = false;
            int searchedRooms = 0;
            float foundTreasure = 0;
            Room* roomBeingSearchedP = theRoomPs[0];
            // we set its searched field to true, and take its treasure
            roomBeingSearchedP->searched = true;
            // we record it in the history
            SearchResults* srP = NULL;
            if((roomBeingSearchedP->treasure <= maxTreasure) && (maxRooms > 0)) {
                srP = (SearchResults*) malloc(sizeof(SearchResults));
                srP->roomNumber= 0;
                srP->treasure = roomBeingSearchedP->treasure;
                //we searched room 0, so it should be enqueued onto the history,
                //and not onto the "things-to-do" queue
                puts("Enqueuing room 0");
                saveSearchResult(historyP, srP);//here we are enqueueing room 0
                foundTreasure = roomBeingSearchedP->treasure;
                searchedRooms = 1;
            }
            // search the rest
            while(!done)
            {   //here we want to find all of the rooms adjacent to the roomBeingSearched,
                //so we look in the adj matrix
                for(int col = 0; (col<nrooms) && !done; col++)
                {
                    //we check whether this room is neighboring
                    printf("checking rooms %d and %d.\n", roomBeingSearchedP->roomNumber, col); fflush(stdout);
                    if(getEdge(adjMP,roomBeingSearchedP->roomNumber, col) == 1) {
                        puts("found an edge"); fflush(stdout);
                        //if so, we check whether room has been searched
                        if(!(theRoomPs[col]->searched))
                        {//if it hasn't been searched
                            printf("Room %d hasn't already been searched.\n", col);
                            //we set it to searched
                            theRoomPs[col]->searched=true;
                            if(((foundTreasure + theRoomPs[col]->treasure) <= maxTreasure) && (searchedRooms < maxRooms))
                                //we check whether we can take the treasure vs. limit
                                //we check whether we've hit the room limit
                            { //we enqueue it for search
                                foundTreasure += theRoomPs[col]->treasure;
                                searchedRooms++;
                                printf("found treasure updated to %f.\n", foundTreasure);
                                printf("enqueuing room %d.\n", col); fflush(stdout);
                                printf("Before enqueuing queue empty reports %d\n", isRoomLLEmpty(searchQ));
                                saveRoom(searchQ, theRoomPs[col]);
                                srP = (SearchResults*) malloc(sizeof(SearchResults));
                                srP->roomNumber = theRoomPs[col]->roomNumber;
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
                    if (searchedRooms >= maxRooms)
                    {
                        done = true;
                        puts("Done by rooms");
                    }
                }//scan for all possible rooms to search from this room
                //time to get the next room
                if(isRoomLLEmpty(searchQ))
                {
                    done = true;
                    puts("Done by queue empty");
                }
                if(!done)
                {
                    puts("Invoking dequeue");fflush(stdout);
                    roomBeingSearchedP = dequeueLIFO(searchQ);

                }
            }//while search is not done
            //search is now done, time to print the history
            printSearchHistory(historyP);

            // Clean up
            deleteSearchLL(historyP);
            for(int i = 0; i < adjMP->n; i++) {
                Room* currRoom = theRoomPs[i];
                RoomNode* tmp = searchQ;
                bool isInLL = false;
                while ( tmp != NULL && !isInLL) {
                    if(tmp->roomP == currRoom) {
                        isInLL = true;
                    }
                    tmp = (RoomNode*) tmp->next;
                }
                if(!isInLL) {
                    free(theRoomPs[i]);
                    theRoomPs[i] = NULL;
                }
            }
            deleteRoomLL(searchQ);
            free(adjMP->edgesP);
        }
        free(adjMP);
	} //end of else we have good arguments
	return answer;
}

bool readFile(char* filename, int* nrooms, AdjMat* adjMP, Room** theRoomPs)
{
	bool ok = true;
	// the file tells how many rooms there are
	FILE* fp = fopen(filename, "r"); //read the file
	if (fp == NULL) {
		int errnum = errno;
		fprintf(stderr, "Value of errno: %d\n", errno);
		perror("Error printed by perror");
		fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
		puts("Error! opening file");
		ok = false;
	} else {
	    char inpPtr[40];
		fscanf(fp,"%s", inpPtr);
		*nrooms = (int) strtol(inpPtr, NULL, 10);
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
				fscanf(fp,"%s", inpPtr);
                temp = (int) strtol(inpPtr, NULL, 10);
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
			fscanf(fp,"%s", inpPtr);
			tempTreas = (float) strtod(inpPtr, NULL);
			// make a room and set its treasure
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
		// close here otherwise we would close a NULL file pointer which is invalid
		fclose(fp);
	}
	//the file contains a triangular array of integers, that are either 1 or 0
	return ok;
}



void getInputArgs(int* maxRooms, double* maxTreas, float* maxTreasure) {
    char inStr[6];
    long aL = -1L;
    // if maxRooms still is <= 0 prompt user for maxRooms
    while(*maxRooms <= 0 ) {
        printf("Input Max Rooms: ");
        fgets(inStr, sizeof(inStr), stdin);
        // check for newline at end of input
        if (inStr[strlen(inStr) - 1] == '\n') {
            aL= strtol(inStr, NULL, 10);
            *maxRooms = (int) aL;
            printf("Number of rooms is %d\n",*maxRooms);
            fflush(stdout);
        } else {
            printf("The input was too long.\n");
            // Clear buffer (I don't like fflush)
            char c = 'a';
            while ((c = (char) getchar()) != '\n' && c != EOF) {}
        }
    }
    // if maxTreasure still is <=0  prompt user for treasure
    while(*maxTreasure <= 0) {
        printf("Input Max Treasure: ");
        fgets(inStr, sizeof(inStr), stdin);
        // check for newline at end of input
        if (inStr[strlen(inStr) - 1] == '\n') {
            *maxTreas = strtod(inStr, NULL);
            *maxTreasure = (float) *maxTreas;
            printf("Amount of treasure is %f\n", *maxTreasure);
            fflush(stdout);
        } else {
            printf("The input was too long.\n");
            // Clear buffer (I don't like fflush)
            char c = 'a';
            while ((c = (char) getchar()) != '\n' && c != EOF) {}
        }
    }
}

