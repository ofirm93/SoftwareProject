//
// Created by Ofir on 29/03/2017.
//


#include <cstdio>
#include <cstdlib>

#define MAX_PATH_LENGTH 1024

#define MEM_ALC_ERR_MSG "An error occurred - allocation failure\n"

extern "C"{
    #include "main_aux.h"
    #include "SPImageProc.h"
    #include "SPBPriorityQueue.h"
    #include "KNearestSearch.h"
}

char* spGetInputFromUser(const char *command) {
    if(command == NULL)
    {
        return NULL;
    }
    char* input = (char*) malloc(MAX_PATH_LENGTH * sizeof(char));
    if(input == NULL){
        // TODO print memory allocation error
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    printf("%s",command);
    fflush(stdout);
    scanf("%s", input);
    return input;
}

void spNonMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size) {
    if(!queryPath || !config || !indexArray || size<1){
        return;
    }
    printf("\"Best candidates for - %s - are:\n", queryPath);
    for (int i = 0; i < size; ++i) {
        char path[1024];
        SP_CONFIG_MSG msg = spConfigGetImagePath(path, config, indexArray[i]);
        printf("%s\n", path);
    }
}

void spMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size) {
    if(!queryPath || !config || !indexArray || size<1){
        return;
    }
    sp::ImageProc s = sp::ImageProc(config);
    for (int i = 0; i < size; ++i) {
        char path[1024];
        SP_CONFIG_MSG msg = spConfigGetImagePath(path, config, indexArray[i]);
        s.showImage(path);
    }
}

int elementByValueComparator(const void *element1, const void *element2){
    double value1 = (*(BPQueueElement*)element1).value;
    double value2 = (*(BPQueueElement*)element2).value;
    if(value1 - value2 == 0){
        return 0;
    }
    else if( value1 - value2 > 0){
        return 1;
    }
    return -1;
}

int* spGetGetBestKMatches(SPKDTree* kdTree, char* queryPath, SPConfig config){
    if(!kdTree || !queryPath || !config){
        // TODO print argument is fault error
        return NULL;
    }
    SP_CONFIG_MSG msg;
    int numOfImages = spConfigGetNumOfImages(config, &msg);
    if(msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return NULL;
    }
    int k = spConfigGetKNN(config, &msg);
    if(msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        // TODO free all resources
        return NULL;
    }
    if(k > numOfImages){
        // TODO print asked to many nearest neighbours
        // TODO free all resources
        return NULL;
    }
    sp::ImageProc s = sp::ImageProc(config);
    int* numOfQueryFeat =(int*) malloc(sizeof(int));
    if(!numOfQueryFeat){
        // TODO print memory allocation error
        // TODO free all resources
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    SPPoint** queryFeat = s.getImageFeatures(queryPath, DEFAULT_INDEX, numOfQueryFeat);
    if(!queryFeat){
        // TODO print query cannot extract features check if the path is correct
        // TODO free all resources
        return NULL;
    }
    int* imageCounter =(int*) malloc(numOfImages * sizeof(int));
    if(!imageCounter){
        // TODO print memory allocation error
        // TODO free all resources
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    for (int i = 0; i < numOfImages; ++i) {
        imageCounter[i] = 0;
    }
    for (int i = 0; i < *numOfQueryFeat; ++i) {
        SPBPQueue* kClose = getKClosestPoints(kdTree, queryFeat[i], k);
        if(!kClose){
            // TODO print error while trying to find best match for a feature
            // TODO free all resources
            return NULL;
        }
        SP_BPQUEUE_MSG bpqMsg;
        BPQueueElement feature;
        while(!spBPQueueIsEmpty(kClose)){
            bpqMsg = spBPQueuePeek(kClose, &feature);
            if(msg == SP_BPQUEUE_INVALID_ARGUMENT || msg == SP_BPQUEUE_EMPTY){
                // TODO print error while trying to find best match for a feature
                // TODO free all resources
                return NULL;
            }
            bpqMsg = spBPQueueDequeue(kClose);
            if(msg == SP_BPQUEUE_INVALID_ARGUMENT || msg == SP_BPQUEUE_EMPTY){
                // TODO print error while trying to find best match for a feature
                // TODO free all resources
                return NULL;
            }
        }
    }
    BPQueueElement* sortingArr =(BPQueueElement*) malloc(numOfImages * sizeof(BPQueueElement)); // initialize helper array
    if (!sortingArr) {
        // TODO print memory allocation error
        // TODO free all resources
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    for (int i = 0; i < numOfImages; ++i) {
        sortingArr[i].index = i;
        sortingArr[i].value = imageCounter[i];
    }
    qsort(sortingArr, (size_t) numOfImages, sizeof(BPQueueElement), elementByValueComparator);
    int* result =(int*) malloc(k * sizeof(int));
    if (!result) {
        // TODO print memory allocation error
        // TODO free all resources
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    for (int i = 0; i < k; ++i) {
        result[i] = sortingArr[k-i].index;
    }
    return result;
}


