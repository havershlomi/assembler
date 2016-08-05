#include "utils.h"
#include "fileHandler.h"

static char entryFileName[MAX_FILE_NAME] = "", externalFileName[MAX_FILE_NAME] = "", objectlFileName[MAX_FILE_NAME] = "";
static FILE  *entryFile, *externalFile, *objFile;
void createFile(const char* fileName){    
    sprintf(entryFileName, "%s.%s",fileName,"ent");
    sprintf(externalFileName, "%s.%s",fileName,"ext");
    sprintf(objectlFileName, "%s.%s",fileName,"ob");

    if((externalFile = fopen(externalFileName,"w")) == NULL){
        printErr("can not create %s output file",externalFileName);
    }

    if((entryFile = fopen(entryFileName,"w")) == NULL){
        printErr("can not create %s output file",entryFileName);
    }

    if((objFile = fopen(objectlFileName,"w")) == NULL){
        printErr("can not create %s output file",objectlFileName);
    }

}

void entryWriteToFile(char * format,...){
    va_list arg;
    if(entryFile){
        va_start(arg, format);
        vfprintf(entryFile, format, arg);
        va_end(arg); 
    }
}
void objWriteToFile(char * format,...){
    va_list arg;
    
    if(objFile){
        va_start(arg, format);
        vfprintf(objFile, format, arg);
        va_end(arg); 
    }
}
void externalWriteToFile(char * format,...){
    va_list arg;
    
    if(externalFile){
        va_start(arg, format);
        vfprintf(externalFile, format, arg);
        va_end(arg); 
    }
}

void closeFiles(){
    if(externalFile)
		fclose(externalFile);
	if(entryFile)
		fclose(entryFile);
    if(objFile)
		fclose(objFile);
}

void deleteFile(){
}
 