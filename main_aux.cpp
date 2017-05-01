//
// Created by Ofir on 29/03/2017.
//


#include <cstdio>
#include <cstdlib>
#include "main_aux.h"

#define MAX_PATH_LENGTH 1024
#define MAX_ERR_MSG_LENGTH 1024

#define ERR_MSG_INVALID_ARG "Error : One of the arguments supplied to the method is invalid."
#define ERR_MSG_INVALID_ARG_IN_METHOD "Error : One of the arguments supplied to the method %s is invalid."
#define ERR_MSG_CANNOT_ALLOCATE_MEM "Error : Couldn't allocate needed memory."
#define ERR_MSG_GET_IMG_PATH "spConfigGetImagePath()"
#define ERR_MSG_INDEX_OUT_OF_RANGE "Error : The index %d of a best candidate is out of range"
#define ERR_MSG_GENERAL "General error. Unexpectedly, the program reached to a code where it shouldn't supposed to."
#define ERR_MSG_MORE_IMG_THAN_AVAILABLE "Error : asked for %d closest images while there are only %d images at all."
#define ERR_MSG_CANNOT_EXT_FEAT "Error : Couldn't extract feature from the given image.\n Please check if Path : %s is correct."
#define ERR_MSG_CANNOT_FIND_CLOSE_FEAT "Error : Couldn't find the closest features to a feature."
#define ERR_MSG_BPQUEUE_PEEK "spBPQueuePeek()"
#define ERR_MSG_BPQUEUE_DEQUEUE "spBPQueueDequeue()"
#define ERR_MSG_CANNOT_PEEK_EMPTY "Error : Couldn't peek to the queue top because it's empty."
#define ERR_MSG_CANNOT_DEQUEUE_EMPTY "Error : Couldn't dequeue element from the queue because it's empty."
#define ERR_MSG_MTHD_GET_IMG_PATH_DATA "spConfigGetImagePathWithData()"
#define ERR_MSG_FAIL_PRINT_TO_FILE "Error : Failed to print to file."
#define ERR_MSG_FAIL_SCAN_FILE "Error : Failed reading from features file.\nPlease check if Path : %s is correct."

extern "C"{
//    #include "SPBPriorityQueue.h"
    #include "KNearestSearch.h"
}

char* spGetInputFromUser(const char *command) {
    if(command == NULL) {
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    char* input = (char*) malloc(MAX_PATH_LENGTH * sizeof(char));
    if(input == NULL){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return NULL;
    }
    printf("%s",command);
    fflush(stdout);
    scanf("%s", input);
    return input;
}

void spNonMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size) {
    if(!queryPath || !config || !indexArray || size<1){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return;
    }
    printf("Best candidates for - %s - are:\n", queryPath);
    char errorMsg[MAX_ERR_MSG_LENGTH];
    for (int i = 0; i < size; ++i) {
        char path[1024];
        SP_CONFIG_MSG msg = spConfigGetImagePath(path, config, indexArray[i]);
        switch (msg){
            case SP_CONFIG_INVALID_ARGUMENT:
                sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_GET_IMG_PATH);
                spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                return;
            case SP_CONFIG_INDEX_OUT_OF_RANGE:
                sprintf(errorMsg, ERR_MSG_INDEX_OUT_OF_RANGE, indexArray[i]);
                spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                return;
            case SP_CONFIG_SUCCESS:
                break;
            default:
                spLoggerPrintError(ERR_MSG_GENERAL, __FILE__, __func__, __LINE__);
                return;
        }
        printf("%s\n", path);
    }
}

void spMinimalGUI(SPConfig config, char* queryPath, int* indexArray, int size) {
    if(!queryPath || !config || !indexArray || size<1){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return;
    }
    sp::ImageProc s = sp::ImageProc(config);
    char errorMsg[MAX_ERR_MSG_LENGTH];
    for (int i = 0; i < size; ++i) {
        char path[MAX_PATH_LENGTH];
        SP_CONFIG_MSG msg = spConfigGetImagePath(path, config, indexArray[i]);
        switch (msg){
            case SP_CONFIG_INVALID_ARGUMENT:
                sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_GET_IMG_PATH);
                spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                return;
            case SP_CONFIG_INDEX_OUT_OF_RANGE:
                sprintf(errorMsg, ERR_MSG_INDEX_OUT_OF_RANGE, indexArray[i]);
                spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                return;
            case SP_CONFIG_SUCCESS:
                break;
            default:
                spLoggerPrintError(ERR_MSG_GENERAL, __FILE__, __func__, __LINE__);
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

int* spGetBestKMatches(SPKDTree *kdTree, char *queryPath, SPConfig config, int numOfImages, int k){
    if(!kdTree || !queryPath || !config || numOfImages < 0 || k < 0){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if(k > numOfImages){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_MORE_IMG_THAN_AVAILABLE, k, numOfImages);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        return NULL;
    }
    sp::ImageProc s = sp::ImageProc(config);
    int numOfQueryFeat;
    SPPoint** queryFeat = s.getImageFeatures(queryPath, DEFAULT_INDEX, &numOfQueryFeat);
    if(!queryFeat){
        char errorMsg[MAX_ERR_MSG_LENGTH];
        sprintf(errorMsg, ERR_MSG_CANNOT_EXT_FEAT, queryPath);
        spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
        return NULL;
    }
    int* imageCounter =(int*) malloc(numOfImages * sizeof(int));
    if(!imageCounter){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        spDestroySPPointArray(queryFeat, numOfQueryFeat);
        return NULL;
    }
    for (int i = 0; i < numOfImages; ++i) {
        imageCounter[i] = 0;
    }
    for (int i = 0; i < numOfQueryFeat; ++i) {
        SPBPQueue* kClose = getKClosestPoints(kdTree, queryFeat[i], k);
        if(!kClose){
            spLoggerPrintError(ERR_MSG_CANNOT_FIND_CLOSE_FEAT, __FILE__, __func__, __LINE__);
            spDestroySPPointArray(queryFeat, numOfQueryFeat);
            free(imageCounter);
            return NULL;
        }
        SP_BPQUEUE_MSG bpqMsg;
        BPQueueElement feature;
        while(!spBPQueueIsEmpty(kClose)){
            bpqMsg = spBPQueuePeek(kClose, &feature);
            switch (bpqMsg){
                case SP_BPQUEUE_INVALID_ARGUMENT:
                    char errorMsg[MAX_ERR_MSG_LENGTH];
                    sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_BPQUEUE_PEEK);
                    spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_EMPTY:
                    spLoggerPrintError(ERR_MSG_CANNOT_PEEK_EMPTY, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_SUCCESS:
                    break;
                default:
                    spLoggerPrintError(ERR_MSG_GENERAL, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
            }
            if(feature.index < 0 || feature.index > numOfImages){
                char errorMsg[MAX_ERR_MSG_LENGTH];
                sprintf(errorMsg, ERR_MSG_INDEX_OUT_OF_RANGE, feature.index);
                spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                spDestroySPPointArray(queryFeat, numOfQueryFeat);
                free(imageCounter);
                spBPQueueDestroy(kClose);
                return NULL;
            }
            imageCounter[feature.index] = imageCounter[feature.index] + 1;
            bpqMsg = spBPQueueDequeue(kClose);
            switch (bpqMsg){
                case SP_BPQUEUE_INVALID_ARGUMENT:
                    char errorMsg[MAX_ERR_MSG_LENGTH];
                    sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_BPQUEUE_DEQUEUE);
                    spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_EMPTY:
                    spLoggerPrintError(ERR_MSG_CANNOT_DEQUEUE_EMPTY, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
                case SP_BPQUEUE_SUCCESS:
                    break;
                default:
                    spLoggerPrintError(ERR_MSG_GENERAL, __FILE__, __func__, __LINE__);
                    spDestroySPPointArray(queryFeat, numOfQueryFeat);
                    free(imageCounter);
                    spBPQueueDestroy(kClose);
                    return NULL;
            }
        }
        spBPQueueDestroy(kClose);
    }

    spDestroySPPointArray(queryFeat, numOfQueryFeat);
    BPQueueElement* sortingArr =(BPQueueElement*) malloc(numOfImages * sizeof(BPQueueElement)); // initialize helper array
    if (!sortingArr) {
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        free(imageCounter);
        return NULL;
    }
    for (int i = 0; i < numOfImages; ++i) {
        sortingArr[i].index = i;
        sortingArr[i].value = imageCounter[i];
    }
    free(imageCounter);
    qsort(sortingArr, (size_t) numOfImages, sizeof(BPQueueElement), elementByValueComparator);
    int* result =(int*) malloc(k * sizeof(int));
    if (!result) {
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        free(sortingArr);
        return NULL;
    }
    for (int i = 0; i < k; ++i) {
        result[i] = sortingArr[numOfImages - i - 1].index;
    }
    free(sortingArr);
    return result;
}

void spDestroySPPointArray(SPPoint** array, int size){
    if(!array || size < 0){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return;
    }
    for (int i = 0; i < size; ++i) {
        spPointDestroy(array[i]);
    }
    free(array);
}

void spDestroySPPoint2DimArray(SPPoint*** gallery, int* numOfFeatArray, int sizeOfGallery){
    if(!gallery|| !numOfFeatArray){
        return;
    }
    for(int i = 0; i< sizeOfGallery; i++){
        spDestroySPPointArray(gallery[i],numOfFeatArray[i]);
    }
    free(gallery);
}


SPPoint** ExtractionModeAct(char* directory, char* imagePrefix, char* imageSuffix,
                            int spNumOfImages, sp::ImageProc spIp, int* totalNumOfFeatures, int spPCADimension){
    if(!directory || !imagePrefix || !imageSuffix || !totalNumOfFeatures){
        spLoggerPrintError(ERR_MSG_INVALID_ARG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    char featSuffix[7] = ".feats";
    int* numOfFeatures = (int*) malloc(sizeof(int));
    if(!numOfFeatures){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return NULL;
    }
    int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
    if(!numOfFeatArray){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        free(numOfFeatures);
        return NULL;
    }
    int totalNumOfFeat = 0;
    int checker = 0;
    SPPoint*** gallery = (SPPoint***) malloc ( sizeof(*gallery) * spNumOfImages ) ;
    if(!gallery){
        free(numOfFeatures);
        free(numOfFeatArray);
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return NULL;
    }
    for(int i=0; i<spNumOfImages;i++){
        //getting the paths
        char imgPath[MAX_PATH_LENGTH];
        SP_CONFIG_MSG msg = spConfigGetImagePathWithData(imgPath, directory, imagePrefix, i, imageSuffix);
        if(msg != SP_CONFIG_SUCCESS){

            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            return NULL;
        }

        char filePath [MAX_PATH_LENGTH];
        msg = spConfigGetImagePathWithData(filePath, directory, imagePrefix, i, featSuffix);
        if(msg != SP_CONFIG_SUCCESS){

            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            return NULL;
        }

        const char * destPtr = filePath;
        const char writeMode = 'w';
        FILE* file = fopen(destPtr, &writeMode);
        if(file == NULL)
        {
            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, "Error : Couldn't open a file.\nPlease check if Path : %s is correct.", destPtr);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            return NULL;
        }
        SPPoint** imgFeatures = spIp.getImageFeatures(imgPath,i,numOfFeatures);
        if(imgFeatures == NULL){
            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, ERR_MSG_CANNOT_EXT_FEAT, imgPath);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            fclose(file);
            free(numOfFeatures);
            free(numOfFeatArray);
            return NULL;
        }
        for(int k = 0 ; k<*numOfFeatures; k++){
            if(!imgFeatures[k]){
                spDestroySPPointArray(imgFeatures,k);
                fclose(file);
                free(numOfFeatures);
                free(numOfFeatArray);
                free(gallery);
                return NULL;
            }
        }
        numOfFeatArray[i] = *numOfFeatures;
        gallery[i] = imgFeatures;
        totalNumOfFeat += *numOfFeatures;
        checker = fprintf(file,"%d,", *numOfFeatures); //check positive
        if(checker < 0){
            spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            fclose(file);
            free(numOfFeatures);
            free(numOfFeatArray);
            return NULL;
        }


        for (int k=0; k<*numOfFeatures;k++){
            for( int j=0; j<spPCADimension; j++){
                checker = fprintf(file,"%f,",spPointGetAxisCoor(imgFeatures[k],j));
                if(checker < 0){
                    spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
                    spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
                    fclose(file);
                    free(numOfFeatures);
                    free(numOfFeatArray);
                    return NULL;
                }
            }
        }
        fclose(file);
    }

    SPPoint** allImageFeatures = (SPPoint**) malloc(totalNumOfFeat * sizeof(*allImageFeatures));
    if(!allImageFeatures){
        spLoggerPrintError(ERR_MSG_FAIL_PRINT_TO_FILE, __FILE__, __func__, __LINE__);
        spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
        free(numOfFeatures);
        free(numOfFeatArray);
        return NULL;
    }
    int k = 0;
    for(int i=0; i<spNumOfImages;i++){
        for(int j = 0; j < numOfFeatArray[i]; j++){
            allImageFeatures[k] = gallery[i][j];
            k++;
        }
    }
    *totalNumOfFeatures = totalNumOfFeat;
    free(numOfFeatures);
    free(numOfFeatArray);
    free(gallery);//NOT destroy - we need the points!
    return allImageFeatures;
}


SPPoint** NonExtractionModeAct(char* directory, char* imagePrefix,
                               int spNumOfImages, int* totalNumOfFeatures,  int spPCADimension){
    if(!directory || !imagePrefix || !totalNumOfFeatures){
        return NULL;
    }
    const char readMode = 'r';
    int checker = 0;
    //creating the returned variable
    SPPoint*** gallery = (SPPoint***) malloc(sizeof(SPPoint**) * spNumOfImages);
    if(gallery == NULL){
        return NULL;
    }
    int* numOfFeatArray = (int*) malloc(sizeof(int) * spNumOfImages);
    if(!numOfFeatArray){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        free(gallery);
        return NULL;

    }
    int totalNumOfFeat = 0;
    //null check
    char featSuffix[7] = ".feats";
    int* numOfFeatures = (int*) malloc(sizeof(int));
    if(numOfFeatures == NULL){
        free(numOfFeatArray);
        free(gallery);
        return NULL;
    }
    double* featValArray = (double*) malloc(sizeof(double)*spPCADimension);
    if(featValArray == NULL){
        free(gallery);
        free(numOfFeatures);
        free(numOfFeatArray);
        return NULL;
    }
    for(int i=0; i<spNumOfImages; i++){
        char filePath [MAX_PATH_LENGTH];

        SP_CONFIG_MSG msg = spConfigGetImagePathWithData(filePath, directory, imagePrefix, i, featSuffix);
        if(msg != SP_CONFIG_SUCCESS){
            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, ERR_MSG_INVALID_ARG_IN_METHOD, ERR_MSG_MTHD_GET_IMG_PATH_DATA);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            free(numOfFeatures);
            free(numOfFeatArray);
            free(featValArray);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            return NULL;
        }
        FILE* file = fopen(filePath, &readMode);
        if(file == NULL)
        {
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            free(featValArray);
            return NULL;
        }
        checker = fscanf(file, "%d,",numOfFeatures);
        if(checker < 0){
            char errorMsg[MAX_ERR_MSG_LENGTH];
            sprintf(errorMsg, ERR_MSG_FAIL_SCAN_FILE, filePath);
            spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            free(featValArray);
            fclose(file);
            return NULL;
        }
        numOfFeatArray[i] = *numOfFeatures;
        totalNumOfFeat += *numOfFeatures;
        gallery[i] = (SPPoint**) malloc(sizeof(SPPoint*) * *numOfFeatures);
        if(gallery[i] == NULL){
            spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
            free(numOfFeatures);
            free(numOfFeatArray);
            free(featValArray);
            fclose(file);
            return NULL;
        }
        for(int k = 0; k<*numOfFeatures;k++){
            for(int j=0; j<spPCADimension; j++){
                checker = fscanf(file, "%lf,",&featValArray[j]);
                if(checker < 0){
                    char errorMsg[MAX_ERR_MSG_LENGTH];
                    sprintf(errorMsg, ERR_MSG_FAIL_SCAN_FILE, filePath);
                    spLoggerPrintError(errorMsg, __FILE__, __func__, __LINE__);
                    spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
                    free(numOfFeatures);
                    free(numOfFeatArray);
                    free(featValArray);
                    return NULL;
                }
            }
            gallery[i][k]= spPointCreate(featValArray,spPCADimension,i); //index of feature is image's index
            if(gallery[i][k] == NULL){
                spDestroySPPoint2DimArray(gallery,numOfFeatArray,i);
                free(numOfFeatures);
                free(numOfFeatArray);
                free(featValArray);
                fclose(file);
                return NULL;
            }
        }
        fclose(file);
    }

    SPPoint** allImageFeatures = (SPPoint**) malloc (totalNumOfFeat * sizeof(SPPoint*) );
    if(!allImageFeatures){
        spDestroySPPoint2DimArray(gallery,numOfFeatArray,spNumOfImages);
        free(numOfFeatures);
        free(numOfFeatArray);
        free(featValArray);
        return NULL;
    }
    int k = 0;
    for(int i=0; i<spNumOfImages; i++){
        for(int j=0; j<numOfFeatArray[i]; j++){
            allImageFeatures[k] = gallery[i][j];
            k++;
        }
    }

    *totalNumOfFeatures = totalNumOfFeat;
    free(featValArray);
    free(numOfFeatures);
    free(numOfFeatArray);
    for(int l=0; l<spNumOfImages; l++){
        free(gallery[l]);
    }
    free(gallery); //NOT destroy - we need the points.
    return allImageFeatures;
}

