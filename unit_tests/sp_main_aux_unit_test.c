//
// Created by Ofir on 29/03/2017.
//

#include <stdlib.h>
#include "../SPConfig.h"
#include "../main_aux.h"

void spTestMinimalGUI(){
    SP_CONFIG_MSG* msg = malloc(sizeof(SP_CONFIG_MSG));
    if(!msg){
        printf("msg allocation fail\n");
        return;
    }
    SPConfig config = spConfigCreate("unit_tests/config_files/good_config.config", msg);
    int indexArray[] = {0,1,2};
    spMinimalGUI(config, "<some query path>", indexArray, 3);
}