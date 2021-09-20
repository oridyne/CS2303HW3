/*
 * tests.c
 *
 *  Created on: Jul 4, 2019
 *      Author: Therese
 */

#include "tests.h"
#include "production.h"
#include "LinkedList.h"


bool tests()
{
	bool answer = false;
	//test the functions that get used
	//order the tests from simplest first -- this produces building blocks for use in more complex tests
	//check how functions are used in production code, this gives clues about how to provide the arguments for the invocation
	bool ok1 = testReadFile();
	bool ok2 = testGotAdjacencyMatrix();
	bool ok3 = testMakeLList();
	bool ok4 = testEnqueue();
	bool ok5 = testRemoveFromList();
	bool ok6 = testPrintHistory();
	bool ok7 = testInputArgs();
	answer = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7;
	//answer = true;
	return answer;
}

bool getYesNo(char* query)
{
    bool answer = true; //so far
    char said = 'x';
    do
    {
        printf("%s (y/n):",query);
        scanf("%1c",&said);
    }while((said != 'y')&&(said!='n'));
    if(said=='n') {
        answer=false;
    } else {
        answer=true;
    }
    return answer;
}

bool testReadFile()
{
	puts("starting testReadFile"); fflush(stdout);
	bool ok = false;
	//the file tells how many rooms there are
	int answer = -1;
	int rightAnswer = 8;

	AdjMat* adjMP = (AdjMat*) malloc(sizeof(AdjMat));
	Room* theRoomPs[10];//addresses for 10 rooms

	char* fname = "houseGraph.txt";
	printf("Sizeof |%s| is %d.\n", fname, (int)strlen(fname));

	ok = readFile("houseGraph.txt", &answer, adjMP, theRoomPs); //read the file
	if(ok)
	{
		if(answer!=rightAnswer)
		{
			puts("test failed on number of rooms");
		}
		else
		{
			puts("read file succeeded on number of rooms");
		}

	}
	else
	{
		puts("test failed on finding file");
	}
	puts("The adjacency matrix");
	for(int i = 0; i<answer; i++)
	{
		for(int j = 0; j<answer; j++)
		{
			printf("%d",getEdge(adjMP, i, j));

		}
		printf("\n");
	}
	puts("The treasure values");
	for(int i = 0; i<answer; i++)
	{
		printf("%f\n", theRoomPs[i]->treasure);
	}
    for (int i = 0; i < adjMP->n; ++i) {
        Room** aRoomP = theRoomPs;
        aRoomP = aRoomP + i;
        free(*aRoomP);
    }
	free(adjMP->edgesP);
	free(adjMP);
	return ok;
}

bool testGotAdjacencyMatrix()
{
    puts("starting testGotAdjacencyMatrix"); fflush(stdout);
    bool ans = true;
	int nrooms = 8;
    FILE* fp = fopen("test.txt", "r"); //read the file
    AdjMat* adjMP = (AdjMat*) malloc(sizeof(AdjMat));
    adjMP->n= 8;
    adjMP->edgesP = (int*) malloc(nrooms * nrooms *sizeof(int));
    init(adjMP);
    int temp = -1;
    char inpPtr[40];
    puts("should show:\nThe adjacency matrix\n01010101\n"
         "10101011\n"
         "01010111\n"
         "10101000\n"
         "01010001\n"
         "10100010\n"
         "01100100\n"
         "11101000\n");
    for(int roomr = 1; roomr<nrooms; roomr++)
    {
        for(int roomc = 0; roomc<roomr; roomc++)
        {
            fscanf(fp,"%s", inpPtr);
            temp = (int) strtol(inpPtr, NULL, 10);
            //now set the value in the adjacency matrix
            if(temp==1) {
                setEdge(adjMP, roomr, roomc);
            }

        }
    }
    puts("The adjacency matrix");
    for(int i = 0; i<nrooms; i++)
    {
        for(int j = 0; j<nrooms; j++)
        {
            printf("%d",getEdge(adjMP, i, j));

        }
        printf("\n");
    }
    free(adjMP->edgesP);
    free(adjMP);

    if(!getYesNo("is this correct?")) {
        ans = false;
    }
    if(ans) {
        puts("test getAdjacencyMatrix passed");
    } else {
        puts("test getAdjacencyMatrix failed");
    }
	return ans;
}

bool testMakeLList()
{
	bool ok = true;
	puts("starting testMakeLList");fflush(stdout);
	//what are the criteria for success for make LList?
	//should be able to make one, add data to it, get the data back
	//test case 1:
	RoomNode* theListP = makeEmptyRoomLL();
	bool rightAnswer = true;
	bool answer = isRoomLLEmpty(theListP);
	if(answer!=rightAnswer)
	{
		ok = false;
	}
    deleteRoomLL(theListP);
	//test case 2:
	//TODO more test cases here

	if(ok)
	{
		puts("test make LList did pass");
	}
	else
	{
		puts("test make LList did not pass.");
	}

	return ok;
}
bool testEnqueue()
{
	bool ok = true;
	float treasureL[8] = {7.0f, 1.5f, 3.3f, 2.1f, 13.0f, 15.7f, 14.9f, 1.2f};
    float treasureSubT[8] = {7.0f, 8.5f, 11.8f, 13.9f, 26.9f, 42.6f, 57.5f, 58.7f};
    SearchResultNode* historyP = makeEmptySearchLL();
    float treasureTotal = 0.0f;
    for(int i = 0; i < 8; i++) {
        treasureTotal+=treasureL[i];
        SearchResults* srP = (SearchResults*) malloc(sizeof(SearchResults));
        srP->roomNumber = i;
        srP->treasure = treasureTotal;
        saveSearchResult(historyP, srP);
    }
    SearchResultNode* tempS = historyP;
    for(int i = 0; (i < 8) && tempS != NULL; i++) {
        if(tempS->searchP != NULL) {
            printf("At index %d\nTreasureSubT: %f\nHistoryP treasure: %f\n",i,treasureSubT[i], tempS->searchP->treasure);
            if(fabsf(tempS->searchP->treasure - treasureSubT[i]) >= 0.01f) {
                ok = false;
            }
            tempS = (SearchResultNode*) tempS->next;
        }
    }
    deleteSearchLL(historyP);

    RoomNode* searchQ = makeEmptyRoomLL();

    for(int i = 0; i < 8; i++) {
        Room* newRoom = (Room*) malloc(sizeof(Room));
        //now set the treasures
        newRoom->treasure = treasureL[i];
        newRoom->roomNumber = i;
        newRoom->searched = false;
        saveRoom(searchQ,newRoom);
    }
    RoomNode* tempR = searchQ;
    for(int i = 0; (i < 8) && tempR != NULL; i++) {
        if(tempR->roomP != NULL) {
            printf("At index %d\nTreasureL: %f\nSearchP treasure: %f\n",i,treasureL[i], tempR->roomP->treasure);
            if(fabsf(tempR->roomP->treasure - treasureL[i]) >= 0.01f) {
                ok = false;
            }
            tempR = (RoomNode *) tempR->next;
        }
    }
    deleteRoomLL(searchQ);
    if(ok) {
		puts("testEnqueue did pass");
	} else {
		puts("testEnqueue did not pass.");
	}
	return ok;
}
bool testRemoveFromList()
{
	bool ok = true;
    puts("starting testRemoveFromList");fflush(stdout);
	//cases:
	//1 list is empty:return same list
	//2 list is of length one, and item is present: return empty list
	//3 list is of length one, and item is not present: return same list
	//4 list is longer than one, item is present, at first location: return list starting at second element
	//5 list is longer than one, item is present, not at first location: return list with item removed
	//6 list is longer than one, item is not present: return same list
	RoomNode* case1 = makeEmptyRoomLL();
	Room* pay1 = (Room*) malloc(sizeof(Room));
	pay1->roomNumber = 1;
	RoomNode* ans = removeFromList(case1, pay1);
	if((ans != case1) || (ans->roomP != (Room*)0))
	{
		ok = false;

	}
	printf("testRemove case 1 with %d\n", ok); fflush(stdout);
    saveRoom(case1, pay1);
	//this is case2
	ans = removeFromList(case1, pay1);
	if((ans != case1) || (ans->roomP != (Room*)0))
	{
		ok = false;

	}
	printf("testRemove case 2 with %d\n", ok); fflush(stdout);
	//now case 3
	Room* pay3 = (Room*) malloc(sizeof(Room));
	pay3->roomNumber = 3;
	ans = removeFromList(case1, pay3);
	if(ans != case1)//this is only a partial check for list unchanged
	{
		ok = false;

	}
	free(pay3);
	deleteRoomLL(case1);
	printf("testRemove case 3 with %d\n", ok); fflush(stdout);
	//now case 4
	case1 = makeEmptyRoomLL();
	pay1 = (Room*) malloc(sizeof(Room));
	pay1->roomNumber = 1;
    saveRoom(case1, pay1);
	pay3 = (Room*) malloc(sizeof(Room));
	pay3->roomNumber = 3;
    saveRoom(case1, pay3);
	ans = removeFromList(case1, pay1);

	if(ans == case1) {
		ok = false;

	}
	printf("testRemove case 4 with %d\n", ok); fflush(stdout);
	case1 = ans;
	deleteRoomLL(case1);
	//now case 5
	case1 = makeEmptyRoomLL();
	pay1 = (Room*) malloc(sizeof(Room));
	pay1->roomNumber = 1;
    saveRoom(case1, pay1);
	pay3 = (Room*) malloc(sizeof(Room));
	pay3->roomNumber = 3;
    saveRoom(case1, pay3);
	//puts("trying case 5");fflush(stdout);
	ans = removeFromList(case1, pay3);//ans should be equal to case1
	RoomNode* theNext = (RoomNode*) ans->next; //this is element where pay3 got attached
	Room* check = (Room*) 0;
	if (theNext)
	{
		check = theNext->roomP; //this should be pay3, which should have been removed
	}
	//printf("testRemove returned from case 5\n"); fflush(stdout);
	if((ans != case1) || (check != (Room*)0))//disquiet
	{
		ok = false;

	}
	deleteRoomLL(case1);
	//printf("ans == case1 is %d\n", ans==case1);
	//printf("check != 0 is %d\n", check != (Room*)0);
	printf("testRemove case 5 with %d\n", ok); fflush(stdout);
	//now case 6
	case1 = makeEmptyRoomLL();
	pay1 = (Room*) malloc(sizeof(Room));
	pay1->roomNumber = 1;
    saveRoom(case1, pay1);
	pay3 = (Room*) malloc(sizeof(Room));
	pay3->roomNumber = 3;
    saveRoom(case1, pay3);
	Room* another = (Room*) malloc(sizeof(Room));
	another->roomNumber=2;
	ans = removeFromList(case1, another);
	if((ans != case1))
	{
		ok = false;

	}
	free(another);
    deleteRoomLL(case1);
	printf("testRemove case 6 with %d\n", ok); fflush(stdout);
	return ok;
}


bool testPrintHistory()
{
	bool ok = true;
    float treasureL[8] = {7.0f, 1.5f, 3.3f, 2.1f, 13.0f, 15.7f, 14.9f, 1.2f};
    SearchResultNode* historyP = makeEmptySearchLL();
    float treasureTotal = 0.0f;
    for(int i = 0; i < 8; i++) {
        SearchResults* srP = (SearchResults*) malloc(sizeof(SearchResults));
        srP->roomNumber = i;
        srP->treasure = treasureL[i];
        saveSearchResult(historyP, srP);
    }
    printSearchHistory(historyP);
    deleteSearchLL(historyP);
    if(!getYesNo("Is this correct?")){
        ok=false;
    }
	if(ok)
	{
		puts("testPrintHistory did pass");
	}
	else
	{
		puts("testPrintHistory did not pass.");
	}
	return ok;
}

bool testInputArgs() {
    puts("starting testInputArgs");fflush(stdout);
    bool ok = true;
    int maxRooms = -1;
    double maxTreas = 0;
    float maxTreasure = 0;
    printf("Program input args are maxRooms: %d and maxTreasure: %f\n", maxRooms, maxTreasure);
    getInputArgs(&maxRooms, &maxTreas, &maxTreasure);
    if(maxTreasure <= 0 || maxRooms <= 0) {
        puts("Test case 1 failed");
        ok = false;
    }
    maxRooms = 5;
    maxTreasure = 0;
    printf("Program input args are maxRooms: %d and maxTreasure: %f\n", maxRooms, maxTreasure);
    getInputArgs(&maxRooms, &maxTreas, &maxTreasure);
    if(maxTreasure <= 0 || maxRooms != 5) {
        puts("Test case 2 failed");
        ok = false;
    }
    if(ok) {
        puts("testInputArgs did pass");
    } else {
        puts("testInputArgs did not pass.");
    }
    return ok;
}


