//
// Created by Ofir on 29/03/2017.
//

#include "main_aux.h"
#include <cstdio>
#include "SPImageProc.h"

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