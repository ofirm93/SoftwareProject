#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerLevelFromNum(int levelNum, SP_LOGGER_LEVEL* loggerLevel){
	if(!loggerLevel || levelNum < 1 || levelNum > 4){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	switch (levelNum){
		case 1:
			*loggerLevel = SP_LOGGER_ERROR_LEVEL;
			break;
		case 2:
			*loggerLevel = SP_LOGGER_ERROR_LEVEL;
			break;
		case 3:
			*loggerLevel = SP_LOGGER_ERROR_LEVEL;
			break;
		case 4:
			*loggerLevel = SP_LOGGER_ERROR_LEVEL;
			break;
		default:
			return SP_LOGGER_UNDIFINED;
	}
	return SP_LOGGER_SUCCESS;
}
