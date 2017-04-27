/*
 * KNearestSearch.c
 *
 *  Created on: Mar 24, 2017
 *      Author: user
 */

#include "KNearestSearch.h"
/**
 * KNN - number of neigh
 *
 *
 */



void kNearestNeighbors (KDTreeNode* curr , SPBPQueue* bpq, SPPoint* query){
	bool wentRight = true;
	int nodeDim = spKDTreeNodeGetDim(curr);
	double queryVal = spPointGetAxisCoor(query, nodeDim), nodeVal = spKDTreeNodeGetVal(curr);
	if(!curr || !bpq || !query){
		return;
	}
	if (isLeaf(curr)){
		spBPQueueEnqueue(bpq,spKDTreeNodeGetPointIndex(curr),spPointL2SquaredDistance(query,spKDTreeNodeGetPoint(curr)));
		return;
	}
	if(queryVal <= nodeVal){
		wentRight = false;
		//go left
		kNearestNeighbors (spKDTreeNodeGetLeft(curr) , bpq, query);
	}
	else{
		//go right
		kNearestNeighbors (spKDTreeNodeGetRight(curr) , bpq, query);
	}
	//check if we need to go to the other side because full
	if(!spBPQueueIsFull(bpq)){
		if(wentRight){
			kNearestNeighbors (spKDTreeNodeGetLeft(curr) , bpq, query);
		}
		else{
			kNearestNeighbors (spKDTreeNodeGetRight(curr) , bpq, query);
		}
	}
	//check if we need to go cuz we might improve the points we have
	//assuming queue saves the max... unlike the header
	else if ((nodeVal - spPointGetAxisCoor(query,nodeDim)) * (nodeVal - spPointGetAxisCoor(query,nodeDim))
			< spBPQueueMaxValue(bpq)){
		if(wentRight){
			kNearestNeighbors (spKDTreeNodeGetLeft(curr) , bpq, query);
		}
		else{
			kNearestNeighbors (spKDTreeNodeGetRight(curr) , bpq, query);
		}
	}
}


SPBPQueue* getKClosestPoints(SPKDTree* kdTree, SPPoint* query, int KNN){
	if(!kdTree || !query){
		return NULL;
	}
	SPBPQueue* queue = spBPQueueCreate(KNN);
	if(!queue){
		return NULL;
	}
	kNearestNeighbors(spGetSPKDTreeRoot(kdTree),queue,query);
	return queue;
}



