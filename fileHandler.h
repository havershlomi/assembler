#include<stdarg.h>
#include<stdio.h>

#define MAX_FILE_NAME 256


void createFile(const char* fileName);
void entryWriteToFile(char * format,...);
void objWriteToFile(char * format,...);
void externalWriteToFile(char * format,...);

void closeFiles();

void deleteFile();