//
// Created by ofir on 30/04/17.
//

#include <cstdlib>
#include "../main_aux.h"

extern "C"{

#include "unit_test_util.h"
}

#define ERR_MSG_CANNOT_ALLOCATE_MEM "Error : Couldn't allocate needed memory."

bool spTestMinimalGUI(){
    SP_CONFIG_MSG* msg = (SP_CONFIG_MSG *) malloc(sizeof(SP_CONFIG_MSG));
    if(!msg){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return false;
    }
    SPConfig config = spConfigCreate("./unit_tests/config_files/config_for_main_aux_minimal_gui_tester.config", msg);
    if(*msg != SP_CONFIG_SUCCESS){
        spLoggerPrintError("Error : Failed creating configuration from file", __FILE__, __func__, __LINE__);
        return false;
    }
    int indexArray[] = {0,1,2};
    spMinimalGUI(config, (char *) "<some query path>", indexArray, 3);
    return true;
}

bool spTestNonMinimalGUI(){
    SP_CONFIG_MSG* msg = (SP_CONFIG_MSG *) malloc(sizeof(SP_CONFIG_MSG));
    if(!msg){
        spLoggerPrintError(ERR_MSG_CANNOT_ALLOCATE_MEM, __FILE__, __func__, __LINE__);
        return false;
    }
    SPConfig config = spConfigCreate("./unit_tests/config_files/good_config.config", msg);
    if(*msg != SP_CONFIG_SUCCESS){
        spLoggerPrintError("Error : Failed creating configuration from file", __FILE__, __func__, __LINE__);
        return false;
    }
    int indexArray[] = {0,1,2};
    spNonMinimalGUI(config, (char *) "<some query path>", indexArray, 3);
    return true;
}

int main(){
    SP_LOGGER_MSG loggerMsg = spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
    if(loggerMsg != SP_LOGGER_SUCCESS){
        printf("Failed creating the logger.");
        return 1;
    }
    RUN_TEST( spTestMinimalGUI );
    RUN_TEST( spTestNonMinimalGUI );
    spLoggerDestroy();
    return 0;
}