//
// Created by Ofir on 29/03/2017.
//


#include <cstdio>
#include <cstdlib>

#define MAX_PATH_LENGTH 1024

#define MEM_ALC_ERR_MSG "An error occurred - allocation failure"

extern "C"{
    #include "main_aux.h"
    #include "SPImageProc.h"
    #include "SPBPriorityQueue.h"
    #include "KNearestSearch.h"
}

char* spGetInputFromUser(const char *command) {
    if(command == NULL)
    {
        // TODO print invalid argument error
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
        // TODO print invalid argument error
        return;
    }
    printf("\"Best candidates for - %s - are:\n", queryPath);
    for (int i = 0; i < size; ++i) {
        char path[1024];
        SP_CONFIG_MSG msg = spConfigGetImagePath(path, config, indexArray[i]);
        switch (msg){
            case SP_CONFIG_INVALID_ARGUMENT:
                // TODO print invalid arg in spConfigGetImagePath()
                return;
            case SP_CONFIG_INDEX_OUT_OF_RANGE:
                // TODO print asked to show images which doesn't exists
                return;
            case SP_CONFIG_SUCCESS:
                break;
            default:
                // TODO unexpectedly reached unreachable code
                return;
        }
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
        //TODO copy switch from the method above
        switch (msg){
            case SP_CONFIG_INVALID_ARGUMENT:
                // TODO print invalid arg in spConfigGetImagePath()
                return;
            case SP_CONFIG_INDEX_OUT_OF_RANGE:
                // TODO print asked to show images which doesn't exists
                return;
            case SP_CONFIG_SUCCESS:
                break;
            default:
                // TODO unexpectedly reached unreachable code
                return;
        }
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

int* spGetGetBestKMatches(SPKDTree* kdTree, char* queryPath, SPConfig config, int numOfImages, int k){
    if(!kdTree || !queryPath || !config || numOfImages < 0 || k < 0){
        // TODO print invalid argument error
        return NULL;
    }
    if(k > numOfImages){
        // TODO print asked for too many nearest neighbours
        // TODO free all resources
        return NULL;
    }
    sp::ImageProc s = sp::ImageProc(config);
    int numOfQueryFeat;
    SPPoint** queryFeat = s.getImageFeatures(queryPath, DEFAULT_INDEX, &numOfQueryFeat);
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
    // TODO from this point free imageCounter before return
    for (int i = 0; i < numOfImages; ++i) {
        imageCounter[i] = 0;
    }
    for (int i = 0; i < numOfQueryFeat; ++i) {
        SPBPQueue* kClose = getKClosestPoints(kdTree, queryFeat[i], k);
        if(!kClose){
            // TODO print error while trying to find best match for a feature
            free(imageCounter);
            return NULL;
        }
        SP_BPQUEUE_MSG bpqMsg;
        BPQueueElement feature;
        while(!spBPQueueIsEmpty(kClose)){
            bpqMsg = spBPQueuePeek(kClose, &feature);
            switch (bpqMsg){
                case SP_BPQUEUE_INVALID_ARGUMENT:
                    // TODO print invalid argument error in spBPQueuePeek()
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_EMPTY:
                    // TODO print empty queue error
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_SUCCESS:
                    break;
                default:
                    // TODO print unreachable code
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
            }

            bpqMsg = spBPQueueDequeue(kClose);
            switch (bpqMsg){
                case SP_BPQUEUE_INVALID_ARGUMENT:
                    // TODO print invalid argument error in spBPQueuePeek()
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_EMPTY:
                    // TODO print empty queue error
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_SUCCESS:
                    break;
                default:
                    // TODO print unreachable code
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
            }
        }
        spBPQueueDestroy(kClose);
    }

    BPQueueElement* sortingArr =(BPQueueElement*) malloc(numOfImages * sizeof(BPQueueElement)); // initialize helper array
    if (!sortingArr) {
        // TODO print memory allocation error
        printf(MEM_ALC_ERR_MSG);
        free(imageCounter);
        return NULL;
    }
    // TODO from this point free sortingArr before return
    for (int i = 0; i < numOfImages; ++i) {
        sortingArr[i].index = i;
        sortingArr[i].value = imageCounter[i];
    }
    free(imageCounter);
    qsort(sortingArr, (size_t) numOfImages, sizeof(BPQueueElement), elementByValueComparator);
    int* result =(int*) malloc(k * sizeof(int));
    if (!result) {
        // TODO print memory allocation error
        free(sortingArr);
        printf(MEM_ALC_ERR_MSG);
        return NULL;
    }
    for (int i = 0; i < k; ++i) {
        result[i] = sortingArr[k-i].index;
    }
    free(sortingArr);
    return result;
}

void spDestroySPPointArray(SPPoint** array, int size){
    if(!array || size < 0){
        // TODO print invalid argument error
        return;
    }
    for (int i = 0; i < size; ++i) {
        spPointDestroy(array[i]);
    }
    free(array);
}

