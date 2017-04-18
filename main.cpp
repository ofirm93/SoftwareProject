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
#include "main_aux.h"

extern "C"{
    #include "SPConfig.h"
}

using namespace std;

int main(int argc, const char* argv[]) {
    SP_CONFIG_MSG* msg = (SP_CONFIG_MSG*) malloc(sizeof(SP_CONFIG_MSG));
    bool isDefaultConfig = true;
    SPConfig config;
    if(!msg){
        // TODO alloc problem report
    }
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


    return 0;
}