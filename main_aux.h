//
// Created by Ofir on 29/03/2017.
//

#ifndef SOFTWAREPROJECT_MAIN_AUX_H
#define SOFTWAREPROJECT_MAIN_AUX_H

#define DEFAULT_INDEX 123

extern "C"{
    #include "SPKDTree.h"
}

/**
 * This method gets an output command string displays it to the user, gets a string input and returns it.
 * If it gets a NULL command or allocation fails it does nothing and returns NULL.
 *
 * @param command - The command to be displayed.
 * @return The input string from the user or NULL as mentioned above.
 */
char* spGetInputFromUser(const char *command);

/**
 * This method simulates text based GUI of the results of the best matches.
 * @param config - the configuration file
 * @param queryPath - the path of the query image
 * @param indexArray - the array of the indices of the best matches
 * @param size - the size of the array above
 */
void spNonMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size);

/**
 * This method simulates minimal GUI of the results of the best matches.
 * @param config - the configuration file
 * @param queryPath - the path of the query image
 * @param indexArray - the array of the indices of the best matches
 * @param size - the size of the array above
 */
void spMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size);

/**
 * This method finds the best matches to the query image.
 * @param tree - a tree to find the results based on the algorithm
 * @param queryPath - the path to the query image
 * @param config - the configuration file
 * @param numOfImages - the number of images in the directory
 * @param k - the number og matches to find
 * @return
 * NULL                                             - if one argument is invalid
 * The array of the indices of the best matches     - otherwise
 */
int* spGetBestKMatches(SPKDTree *tree, char *queryPath, SPConfig config, int numOfImages, int k);

/**
 * A destroyer for an array of SPPoints
 * @param array - the array to destroy
 * @param size - the array's size
 */
void spDestroySPPointArray(SPPoint** array, int size);

#endif //SOFTWAREPROJECT_MAIN_AUX_H
