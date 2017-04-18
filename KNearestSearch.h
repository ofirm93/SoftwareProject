/*
 * KNearestSearch.h
 *
 *  Created on: Mar 24, 2017
 *      Author: user
 */

#ifndef KNEARESTSEARCH_H_
#define KNEARESTSEARCH_H_

#include "SPPoint.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"

/**
 * Computes the KNN closest points to the query point
 * Using the kdTree given.
 *
 * @param
 * kdTree - the KDTree of the points
 * query - the query point which we want to get KNN closest points for
 * KNN - number of closest points
 */
SPBPQueue* getKClosestPoints(SPKDTree* kdTree, SPPoint* query, int KNN);


#endif /* KNEARESTSEARCH_H_ */
