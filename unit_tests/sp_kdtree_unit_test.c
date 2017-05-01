
//
// * sp_kdtree_unit_test.c
// *
// *  Created on: Apr 23, 2017
// *      Author: user


#include "../SPKDTree.h"
#include "unit_test_util.h"
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "../KNearestSearch.h"
#include "../SPLogger.h"


#define ERR_MSG_INVALID_ARG "Error : One of the arguments supplied to the method is invalid."
#define MAX_DBG_MSG_LENGTH 1024
void spDestroyPointArray(SPPoint** array, int size){
    if(!array || size < 0){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return;
    }
    for (int i = 0; i < size; ++i) {
        spPointDestroy(array[i]);
    }
    free(array);
}


bool testKDTreeInitAndSearch(){

    int numOfPoints = 5;
    int dimension = 3;
    int KNN = 2;
    SPPoint** pointArr = malloc(numOfPoints * sizeof(SPPoint*));
    if(!pointArr){
        return 0;
    }
    double val[5][3] = {{0, 5, -5}, {1, 4, -4}, {2, 3, -3}, {3, 2, -2}, {0,0,0}};
    for (int i = 0; i < numOfPoints; ++i) {
        pointArr[i] = spPointCreate(val[i], dimension , i);
        if(!pointArr[i]){

        	spDestroyPointArray(pointArr,i);
        }
    }
//    //tree test starts
    SPKDTree* tree = spInitSPKDTree(pointArr,5,3,INCREMENTAL);
    if(!tree){
        spDestroyKDTree(tree);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    inOrderScan(tree);
    SPKDTree* treeSpread = spInitSPKDTree(pointArr,5,3,MAX_SPREAD);
    if(!treeSpread){
        spDestroyKDTree(tree);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    inOrderScan(treeSpread);
    double valArr[3] = {3,2,-4};
    SPPoint* query = spPointCreate(valArr,3,5);
    if(!query){
        spDestroyKDTree(tree);
        spDestroyKDTree(treeSpread);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    SPBPQueue* kClosest = getKClosestPoints(tree,  query, KNN);
    if(!kClosest){
        spPointDestroy(query);
        spDestroyKDTree(tree);
        spDestroyKDTree(treeSpread);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    SPBPQueue* kClosestSpread = getKClosestPoints(treeSpread,  query, KNN);
    if(!kClosestSpread){
        spBPQueueDestroy(kClosest);
        spPointDestroy(query);
        spDestroyKDTree(tree);
        spDestroyKDTree(treeSpread);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    BPQueueElement* element = malloc(sizeof(*element));
    if(!element){
        spBPQueueDestroy(kClosest);
        spBPQueueDestroy(kClosestSpread);
        spPointDestroy(query);
        spDestroyKDTree(tree);
        spDestroyKDTree(treeSpread);
        spDestroyPointArray(pointArr,numOfPoints);
    }
    int index1 =0 ,index2 = 0;
    for(int i=0; i<KNN; i++){
    	spBPQueuePeek(kClosest,element );
    	index1 = element ->index;
    	char dbgMsg[MAX_DBG_MSG_LENGTH];
    	spBPQueuePeek(kClosestSpread,element );
    	index2 = element ->index;
      	sprintf(dbgMsg,"Closest index number %d of Incremental,Spread : %d , %d \n",i +1,index1, index2);
    	spLoggerPrintDebug(dbgMsg,__FILE__,__func__,__LINE__);
    	ASSERT_TRUE(spBPQueueMinValue(kClosest) == spBPQueueMinValue(kClosestSpread));
    	ASSERT_TRUE(index1 == index2);
    	ASSERT_TRUE(spBPQueueDequeue(kClosest) == SP_BPQUEUE_SUCCESS);
    	ASSERT_TRUE(spBPQueueDequeue(kClosestSpread) == SP_BPQUEUE_SUCCESS);
    }
    spBPQueueDestroy(kClosest);
    spBPQueueDestroy(kClosestSpread);
    free(element);
    spPointDestroy(query);
    spDestroyKDTree(tree);
    spDestroyKDTree(treeSpread);
    spDestroyPointArray(pointArr,numOfPoints);
	return true;
}

int main(){
	SP_LOGGER_MSG loggerMsg = spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
	if(loggerMsg != SP_LOGGER_SUCCESS){
		printf("Failed creating the logger.");
		return -1;
	}
	RUN_TEST( testKDTreeInitAndSearch );
	spLoggerDestroy();
	return 0;

}

