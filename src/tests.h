/*
 * tests.h
 *
 *  Created on: Jul 4, 2019
 *      Author: Therese
 */

#ifndef TESTS_H_
#define TESTS_H_


#include "production.h"
#include "AdjMat.h"
#include <math.h>
bool tests();

bool testReadFile();
bool testGotAdjacencyMatrix();
bool testMakeLList();
bool testEnqueue();
bool testRemoveFromList();
bool testPrintHistory();
bool testInputArgs();

#endif /* TESTS_H_ */
