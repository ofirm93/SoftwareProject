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


SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	int verifier = 0;
	if (logger== NULL){ // Was not defined yet
		return SP_LOGGER_UNDIFINED;}
	if (msg == NULL || file == NULL || function == NULL || line < 0) { //Invalid arg, maybe line <=0
		return SP_LOGGER_INVAlID_ARGUMENT;}
	if (logger->isStdOut == true){ // possibly not needed, but else theres no use for the bool. Maybe runs faster?
		verifier = printf("---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }
	if (logger->isStdOut == false){
		verifier = fprintf(logger->outputChannel,"---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }

	if (verifier < 0){
		return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;
}



SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
	int verifier = 0;
	if (logger== NULL){ // Was not defined yet
		return SP_LOGGER_UNDIFINED;}
	if (msg == NULL || file == NULL || function == NULL || line < 0) { //Invalid arg, maybe line <=0
		return SP_LOGGER_INVAlID_ARGUMENT;}
	if (logger->level == SP_LOGGER_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;}
	if (logger->isStdOut == true){ // possibly not needed, but else theres no use for the bool. Maybe runs faster?
		verifier = printf("---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }
	if (logger->isStdOut == false){
		verifier = fprintf(logger->outputChannel,"---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }

	if (verifier < 0){
		return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;
}


SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	int verifier = 0;
	if (logger== NULL){ // Was not defined yet
		return SP_LOGGER_UNDIFINED;}
	if (msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;}
	if (logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL &&
		logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;}
	if (logger->isStdOut == true){ // possibly not needed, but else theres no use for the bool. Maybe runs faster?
		verifier = printf("---INFO---\n- message: %s\n",
		msg);}
	if (logger->isStdOut == false){
		verifier = fprintf(logger->outputChannel,"---INFO---\n- message: %s\n",
		msg); }

	if (verifier < 0){
		return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;


}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	int verifier = 0;
	if (logger== NULL){ // Was not defined yet
		return SP_LOGGER_UNDIFINED;}
	if (msg == NULL || file == NULL || function == NULL || line < 0) { //Invalid arg, maybe line <=0
		return SP_LOGGER_INVAlID_ARGUMENT;}
	if (logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;}
	if (logger->isStdOut == true){ // possibly not needed, but else theres no use for the bool. Maybe runs faster?
		verifier = printf("---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }
	if (logger->isStdOut == false){
		verifier = fprintf(logger->outputChannel,"---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
		file,function,line,msg); }

	if (verifier < 0){
		return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;
	}


SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){

	int verifier = 0;
	if (logger== NULL){ // Was not defined yet
		return SP_LOGGER_UNDIFINED;}
	if (msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;}
	if (logger->isStdOut == true){ // possibly not needed, but else theres no use for the bool. Maybe runs faster?
		verifier = printf("---MSG---\n- message: %s\n",
		msg); } // CHECK MSG
	if (logger->isStdOut == false){
		verifier = fprintf(logger->outputChannel,"---MSG---\n- message: %s\n",
		msg); }

	if (verifier < 0){
		return SP_LOGGER_WRITE_FAIL;}
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
			*loggerLevel = SP_LOGGER_WARNING_ERROR_LEVEL;
			break;
		case 3:
			*loggerLevel = SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
			break;
		case 4:
			*loggerLevel = SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
			break;
		default:
			break;
	}
	return SP_LOGGER_SUCCESS;
}
