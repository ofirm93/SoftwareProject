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

#define INVALID 1
struct sp_kd_tree_node_t
{
	int dim;
	double val;
	KDTreeNode* left;
	KDTreeNode* right;
	SPPoint* data;
};

struct sp_kd_tree_t{
	KDTreeNode* root;
	SPKDArray features;
};

/**
 * *******************
 * KD TREE NODE
 *
 * *******************
 */
KDTreeNode* spInitKDTreeNode(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPPoint* data){

	KDTreeNode* kdNode = malloc(sizeof(KDTreeNode));
	if(!kdNode){
		return NULL;
	}
	kdNode -> dim = dim;
	kdNode -> val = val;
	kdNode -> left = left;
	kdNode -> right = right;
	kdNode -> data = spPointCopy(data);
	return kdNode;
}


bool isLeaf(KDTreeNode* node){
	if(!node){
		return false;
	}
	if(!node->left && !node->right){
		return true;
	}
	else{
		return false;
	}
}

int spKDTreeNodeGetDim(KDTreeNode* node){
	return node->dim ;
}


double spKDTreeNodeGetVal(KDTreeNode* node){
	return node->val ;
}

KDTreeNode* spKDTreeNodeGetLeft(KDTreeNode* node){
	return node->left;
}

KDTreeNode* spKDTreeNodeGetRight(KDTreeNode* node){
	return node->right ;
}

SPPoint* spKDTreeNodeGetPoint(KDTreeNode* node) {
	return node->data;
}

int spKDTreeNodeGetPointIndex(KDTreeNode* node){
	return spPointGetIndex(node->data) ;
}

double spKDTreeNodeGetPointVal(KDTreeNode* node){
	return spPointGetAxisCoor(node->data, node->dim);
}



void spDestroyKDTreeNode(KDTreeNode* node){
	spPointDestroy(node -> data);
	spDestroyKDTreeNode(node->left);
	spDestroyKDTreeNode(node->right);
	free(node);
}



/**
 * *******************
 * KD TREE
 *
 * *******************
 */

KDTreeNode* spGetSPKDTreeRoot(SPKDTree* tree){
	return tree->root;
}


KDTreeNode* spInitSPKDTreeRec(SPKDArray kdArray, SP_KD_SPLIT_MODE splitMethod, int pointDim, int size,
		int prevSplitDim){
	if(!kdArray)
	{
		return NULL;
	}
	if(size == 1)
	{
		SPPoint* singlePoint = spGetSPKDArrayPoint(kdArray,0);
		if(!singlePoint){
			return NULL;
		}
		return spInitKDTreeNode( INVALID, INVALID, NULL, NULL, singlePoint);  //afaik val >=0 so INVALID is invalid. singlepoint OK?
	}
	int splitDim = -1, sizeL = 0, sizeR = 0;
	double median = 0;
	switch (splitMethod){
	case(MAX_SPREAD):;
		double* spreadArr = malloc(pointDim* sizeof(double));
		if(!spreadArr){
			return NULL;
		}
		double tmpMax = -DBL_MAX , tmpMin = DBL_MAX;
		double coor = 0;
		for (int j=0; j<pointDim; j++){
			for(int i=0; i<size; i++){
				SPPoint* point = spGetSPKDArrayPoint(kdArray,i);
				if(!point){
					free(spreadArr);
					return NULL;
				}
				coor = spPointGetAxisCoor(point,j);
				if(tmpMax <  coor) {
					tmpMax = coor;
				}

				if(tmpMin > coor){
					tmpMin = coor;
				}
				free(point); // needed free?
			}
			spreadArr[j] = tmpMax - tmpMin;
			tmpMax = -DBL_MAX;
			tmpMin = DBL_MAX;
		}
		for(int i=0; i<pointDim; i++){
			if(tmpMax < spreadArr[i]){
				tmpMax = spreadArr[i];
				splitDim = i;
			}
		}
		free(spreadArr);
		break;

	case( RANDOM) :;
		splitDim = rand() % pointDim;
		break;

	case( INCREMENTAL) :;
		splitDim = (prevSplitDim + 1)%pointDim;
		break;

	default:
		break;
	}
	SPKDArray* leftAndRight = spSplitSPKDArray(kdArray, splitDim);
	if(!leftAndRight){
		return NULL;
	}
	SPPoint* pointL1 = spGetSPKDArrayPoint(leftAndRight[0],size/2);
	if(!pointL1){
		free(leftAndRight);
		return NULL;
	}
	SPPoint* pointL2 = spGetSPKDArrayPoint(leftAndRight[0],size/2 - 1);
	if(!pointL2){
		free(pointL1);
		free(leftAndRight);
		return NULL;
	}
	SPPoint* pointR = spGetSPKDArrayPoint(leftAndRight[1],size/2 - 1);
	if(!pointR){
		free(pointL2);
		free(pointL1);
		free(leftAndRight);
		return NULL;
	}
	if(size%2 == 1){
		sizeL = size/2 + 1;
		sizeR = size/2;
		median = spPointGetAxisCoor(pointL1,size/2);
	}
	else{
		sizeL = size/2;
		sizeR = size/2;
		median = (spPointGetAxisCoor(pointL2,size/2 - 1) +  spPointGetAxisCoor(pointR,1)) / 2 ;
	}
	spPointDestroy(pointL1);
	spPointDestroy(pointL2);
	spPointDestroy(pointR);
	KDTreeNode* leftNode = spInitSPKDTreeRec(leftAndRight[0],  splitMethod, pointDim, sizeL,
			splitDim);
	if(!leftNode){
		return NULL;
	}
	KDTreeNode* rightNode = spInitSPKDTreeRec(leftAndRight[1], splitMethod, pointDim, sizeR,
			splitDim);
	if(!rightNode){
		spDestroyKDTreeNode(leftNode);
		return NULL;
	}
	return spInitKDTreeNode(splitDim, median, leftNode, rightNode, NULL);

}


SPKDTree* spInitSPKDTree(SPPoint** arr, int size, int pointDim, SP_KD_SPLIT_MODE splitMethod){
	SPKDTree* tree = malloc(sizeof(SPKDTree));
	if(!tree){
		return NULL;
	}
	SPKDArray kdArray =  spInitSPKDArray(arr, size);
	if(!kdArray){
		free(tree);
		return NULL;
	}
	tree->root = spInitSPKDTreeRec(kdArray, splitMethod, pointDim, size, -1);
	if(!tree->root){
		free(tree);
		//TODO destroy spkdarray
		return NULL;
	}
	return tree;
}

