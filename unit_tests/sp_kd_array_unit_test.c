//
// Created by Ofir on 22/03/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "../SPKDArray.h"
#include "../SPPoint.c"


int main(int argc, const char* argv[]) {
    for(int i=0; i< argc; i++){
        printf("%s\n",argv[i]);
    }
    int numOfPoints = 4;
    int dimension = 3;
    int coordinate = 2;
    SPPoint** pointArr = malloc(numOfPoints * sizeof(SPPoint*));
    if(!pointArr){
        return 0;
    }
    double val[4][3] = {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}};
    for (int i = 0; i < numOfPoints; ++i) {
        pointArr[i] = spPointCreate(val[i], dimension , i);
    }
    SPKDArray kdArr = spInitSPKDArray(pointArr, numOfPoints);
    printf("The big array's details are :\n");
    spPrintKDArrayDetails(kdArr);
    SPKDArray* twoArrays = spSplitSPKDArray(kdArr, coordinate);
    printf("\n\nThe left array's details are :\n");
    spPrintKDArrayDetails(twoArrays[0]);
    printf("\n\nThe right array's details are :\n");
    spPrintKDArrayDetails(twoArrays[1]);
    for (int i = 0; i < numOfPoints; ++i) {
        spPointDestroy(pointArr[i]);
    }
    free(pointArr);
    return 0;
}
