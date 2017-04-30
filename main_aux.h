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
 * TODO write documentation
 */
void spNonMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size);

/**
 * TODO write documentation
 */
void spMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size);

/**
 * TODO write documentation
 */
int* spGetGetBestKMatches(SPKDTree* tree, char* queryPath, SPConfig config, int numOfImages, int k);

/**
 * TODO write documentation
 */
void spDestroySPPointArray(SPPoint** array, int size);

#endif //SOFTWAREPROJECT_MAIN_AUX_H
