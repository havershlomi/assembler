#include<stdarg.h>
#include<stdio.h>

#define MAX_FILE_NAME 256

/* get file name and create all the putput files */
void createFile(const char* fileName);

/* write to the entry file */
void entryWriteToFile(char * format,...);

/* write to the object file */
void objWriteToFile(char * format,...);

/* write tot the external file */
void externalWriteToFile(char * format,...);

/* close all the files */
void closeFiles();

/* delet all the files */
void deleteFiles();