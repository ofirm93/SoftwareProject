//
// Created by Ofir on 22/03/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "../SPKDArray.h"
#include "../SPPoint.c"
#include "../SPLogger.h"


void testSplitSPKDArray(){
    int numOfPoints = 4;
    int dimension = 3;
    int coordinate = 2;
    SPPoint** pointArr = malloc(numOfPoints * sizeof(SPPoint*));
    if(!pointArr){
        return;
    }
    double val[4][3] = {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}};
    for (int i = 0; i < numOfPoints; ++i) {
        pointArr[i] = spPointCreate(val[i], dimension , i);
    }
    SPKDArray kdArr = spInitSPKDArray(pointArr, numOfPoints);
    char mainTitle[1024] = "The big array's details are :\n";
    char* mainMsg = spPrintKDArrayDetails(kdArr);
    SPKDArray* twoArrays = spSplitSPKDArray(kdArr, coordinate);
    char leftTitle[1024] = "\n\nThe left array's details are :\n";
    char* leftMsg = spPrintKDArrayDetails(twoArrays[0]);
    char rightTitle[1024] = "s\n\nThe right array's details are :\n";
    char* rightMsg = spPrintKDArrayDetails(twoArrays[1]);
    char msg[20000];
    sprintf(msg, "%s%s%s%s%s%s", mainTitle, mainMsg, leftTitle, leftMsg, rightTitle, rightMsg);
    spLoggerPrintDebug(msg, __FILE__, __func__, __LINE__);
    for (int i = 0; i < numOfPoints; ++i) {
        spPointDestroy(pointArr[i]);
    }
    free(pointArr);
}
int main(int argc, const char* argv[]) {
    testSplitSPKDArray();
    return 0;
}
