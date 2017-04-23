//============================================================================
// Name        : SPCBIR.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define MAX_PATH_LENGTH 1024

#define QRY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

extern "C"{
    #include "SPConfig.h"
    #include "main_aux.h"
    #include "ExtModeForMain.h"
}

using namespace std;

int main(int argc, const char* argv[]) {
    SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(SP_CONFIG_MSG));
    if(!msg) {
        // TODO alloc problem report
        return 1;
    }
    bool isDefaultConfig = true;
    SPConfig config;

    if(argc == 3 && strcmp(argv[1], "-c") == 0){
        isDefaultConfig = false;
        config = spConfigCreate(argv[2],msg);
    }
    else if(argc == 1){
        config = spConfigCreate("spcbir.config", msg);
    }
    else{
        free(msg);
        printf("\"Invalid command line : use -c <config_filename>\"\n");
        return 0;
    }
    switch (*msg){
        case SP_CONFIG_MISSING_DIR:break;
        case SP_CONFIG_MISSING_PREFIX:break;
        case SP_CONFIG_MISSING_SUFFIX:break;
        case SP_CONFIG_MISSING_NUM_IMAGES:break;
        case SP_CONFIG_CANNOT_OPEN_FILE :
            if(isDefaultConfig){
                printf("\"The default configuration file spcbir.config couldn’t be open\"\n");
                free(msg);
                return 0;
            }
            else{
                printf("\"The configuration file %s couldn’t be open\"\n", argv[2]);
                free(msg);
                return 0;
            }
        case SP_CONFIG_ALLOC_FAIL:break;
        case SP_CONFIG_INVALID_INTEGER:break;
        case SP_CONFIG_INVALID_STRING:break;
        case SP_CONFIG_INVALID_ARGUMENT :
            break;
        case SP_CONFIG_INDEX_OUT_OF_RANGE:break;
        case SP_CONFIG_SUCCESS:break;
    }
    bool isExtractionMode = spConfigIsExtractionMode(config, msg);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 2;
    }
    // Get all the fields required to extract features.
    char imagesDirectory[MAX_PATH_LENGTH];
    *msg = spConfigGetImagesDirectory(config, imagesDirectory);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 3;
    }
    char imagesPrefix[MAX_PATH_LENGTH];
    *msg = spConfigGetImagesPrefix(config, imagesPrefix);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 4;
    }
    char imagesSuffix[MAX_PATH_LENGTH];
    *msg = spConfigGetImagesSuffix(config, imagesSuffix);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 5;
    }
    int numOfImages = spConfigGetNumOfImages(config, msg);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 6;
    }
    int numOfFeatures = spConfigGetNumOfFeatures(config, msg);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 7;
    }
    int dim = spConfigGetPCADim(config, msg);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 8;
    }
    SP_KD_SPLIT_MODE kdSplitMode;
    *msg = spConfigGetKDTreeSplitMethod(config, &kdSplitMode);
    if(*msg == SP_CONFIG_INVALID_ARGUMENT){
        // TODO problem with arguments
        return 9;
    }
    SPPoint*** features;

    if(isExtractionMode){
        // TODO extract features
        sp::ImageProc s = sp::ImageProc(config);
        // TODO bar should alter the method so it would be correct
        features = ExtractionModeAct(imagesDirectory, imagesPrefix, imagesSuffix, numOfImages, numOfFeatures, s);

    } else{
        features = NonExtractionModeAct(imagesDirectory, imagesPrefix, imagesSuffix, numOfImages, numOfFeatures);
    }
    if(!features){
        // TODO Error in extracting features
        return 8;
    }

    SPPoint** gallery = NULL; // TODO flatten features from 3d to only 2d array
    int gallerySize = 0; // TODO Get features number
    SPKDTree* tree = spInitSPKDTree(gallery, gallerySize, dim, kdSplitMode);
    bool isEnded = false;
    char* queryStr;
    while(!isEnded){
        queryStr = spGetInputFromUser(QRY_IMG_MSG);
        if(strcmp(queryStr, "<>") == 0){
            printf(EXIT_MSG);
            isEnded = true;
        } else{
            int* KNN = spGetGetBestKMatches(tree, queryStr, config);
            if(KNN){
                bool isGUIMode = spConfigMinimalGui(config, msg);
                if(*msg == SP_CONFIG_INVALID_ARGUMENT){
                    // TODO problem with arguments
                    //TODO free memory
                    return 10;
                }
                int k = spConfigGetKNN(config, msg);
                if(*msg == SP_CONFIG_INVALID_ARGUMENT){
                    // TODO problem with arguments
                    // TODO free all resources
                    return 11;
                }
                if(isGUIMode){
                    spMinimalGUI(config,queryStr,KNN,k);
                }
                else{
                    spNonMinimalGUI(config, queryStr, KNN, k);
                }
            }
        }
        free(queryStr);
    }
    return 0;
}