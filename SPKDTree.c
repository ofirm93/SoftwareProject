/*
 * SPKDTree.c
 *
 *  Created on: Mar 18, 2017
 *      Author: user
 */

#include "SPKDTree.h"
//#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

struct sp_kd_tree_node_t
{
	int dim;
	double val;
	SPKDTree* left;
	SPKDTree* right;
	SPPoint** data;
};

struct sp_kd_tree_t{
	KDTreeNode* root;
	SPKDArray features;
};

KDTreeNode* spInitKDTreeNode(int dim, double val, SPKDTree* left, SPKDTree* right, SPPoint** data, int numOfPoints){
	if( data == NULL){
		return NULL;
	}
	KDTreeNode* kdNode = malloc(sizeof(KDTreeNode));
	if(!kdNode){
		return NULL;
	}
	kdNode -> dim = dim;
	kdNode -> val = val;
	kdNode -> left = left;
	kdNode -> right = right;
	for(int i=0; i<numOfPoints; i++){
		kdNode -> data[i] = spPointCopy(data[i]);
	}
	return kdNode;
}


KDTreeNode* spInitSPKDTreeRec(SPKDArray kdArray, double* spreadArr, SP_KD_SPLIT_MODE splitMethod, int pointDim, int size){
	if(!kdArray)
	{
		return NULL;
	}
	if(size == 1)
	{
		SPPoint* singlePoint = spGetSPKDArrayPoint(kdArray,0);
		return spInitKDTreeNode( -1, -1, NULL, NULL, &singlePoint ,1);  //afaik val >=0 so -1 is invalid. singlepoint OK?
	}

	switch (splitMethod){
	case(MAX_SPREAD):

		break;

	case(RANDOM) :



	case(INCREMENTAL) :




	default:
		break;
	}
	return NULL;
}

SPKDTree* spInitSPKDTree(SPPoint** arr, int size, int pointDim, SP_KD_SPLIT_MODE splitMethod){
	SPKDTree* tree = malloc(sizeof(SPKDTree));
	if(!tree){
		return NULL;
	}
	SPKDArray kdArray =  spInitSPKDArray(arr, size);
	double* spreadArr = NULL;
	//Create the spread here to prepare
	if(splitMethod == MAX_SPREAD){
		spreadArr = malloc(pointDim* sizeof(double));
		double tmpMax = -DBL_MAX , tmpMin = DBL_MAX;
		double coor = 0;
		for (int j=0; j<pointDim; j++){
			for(int i=0; i<size; i++){
				coor = spPointGetAxisCoor(arr[i],j);
				if(tmpMax <  coor) {
					tmpMax = coor;
				}

				if(tmpMin > coor){
					tmpMin = coor;
				}
			}
			spreadArr[j] = tmpMax - tmpMin;
			tmpMax = -DBL_MAX;
			tmpMin = DBL_MAX;

		}
	}
	tree->root = spInitSPKDTreeRec(kdArray, spreadArr, splitMethod, pointDim, size);
	return tree;
}

