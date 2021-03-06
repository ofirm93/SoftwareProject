//
// * sp_kdtree_unit_test.c
// *
// *  Created on: Apr 23, 2017
// *      Author: user


#include "../SPKDtree.h"
#include "unit_test_util.h"
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "../KNearestSearch.h"
//testing KDNode Methods


bool testAllKDNodeMethods(SPLogger log){
	double arr1 [3] = {1,2,3};
	SPPoint* leafPoint = spPointCreate( arr1 , 3, 1);
	KDTreeNode* leafy = spInitKDTreeNode(5,2,NULL,NULL,leafPoint);
	ASSERT_TRUE(!spInitKDTreeNode(5,127.5,NULL,NULL,NULL));
	ASSERT_TRUE(!spInitKDTreeNode(5,127.5,NULL,leafy,leafPoint));
	return true;
}


bool testKDTreeInitAndSearch(){
//    int numOfPoints = 5;
//    int dimension = 3;
//    int KNN = 2;
//    SPPoint** pointArr = (SPPoint**) malloc(numOfPoints * sizeof(SPPoint*));
//    if(!pointArr){
//        return 0;
//    }
//    double val[5][3] = {{2, 3, -3}, {0, 1, -1}, {3, 2, -3}, {3, 2, -2} , {1,2,3}};
//    for (int i = 0; i < numOfPoints; ++i) {
//        pointArr[i] = spPointCreate(val[i], dimension , i);
//    }

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
    }
//    //tree test starts
    SPKDTree* tree = spInitSPKDTree(pointArr,5,3,INCREMENTAL);
    ASSERT_TRUE(tree != NULL);
    inOrderScan(tree);
    SPKDTree* treeSpread = spInitSPKDTree(pointArr,5,3,MAX_SPREAD);
    ASSERT_TRUE(treeSpread != NULL);
    inOrderScan(treeSpread);
    double valArr[3] = {3,2,-4};
    SPPoint* query = spPointCreate(valArr,3,5);
    SPBPQueue* kClosest = getKClosestPoints(tree,  query, KNN);
    SPBPQueue* kClosestSpread = getKClosestPoints(treeSpread,  query, KNN);
    BPQueueElement* element = malloc(sizeof(*element));
    ASSERT_TRUE(element);
    int index1 =0 ,index2 = 0;
    for(int i=0; i<KNN; i++){
    	spBPQueuePeek(kClosest,element );
    	index1 = element ->index;
    	printf("closest index: %d",index1);
    	spBPQueuePeek(kClosestSpread,element );
    	index2 = element ->index;
    	ASSERT_TRUE(spBPQueueMinValue(kClosest) == spBPQueueMinValue(kClosestSpread));
    	ASSERT_TRUE(index1 == index2);
    	ASSERT_TRUE(spBPQueueDequeue(kClosest) == SP_BPQUEUE_SUCCESS);
    	ASSERT_TRUE(spBPQueueDequeue(kClosestSpread) == SP_BPQUEUE_SUCCESS);
    }
	return true;
}

int main(){
	RUN_TEST( testKDTreeInitAndSearch );
	return 1;

}
