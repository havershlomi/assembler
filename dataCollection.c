#include "dataCollection.h"
#include "dataStructure.h"
#include "fileHandler.h"
#include "binaryConvertor.h"
#include "utils.h"

/* dc counter */
static int dc = 0;
/* data collection */
int dataCollection[1000];

int addStringData(char* data)
{
    int i = 0, currentDc = dc, length = strlen(data);
    for(i = 0; i < length;i++){
        dataCollection[dc] = data[i];
        dc++;
    }
    dataCollection[dc] = '\0';
    dc++;
    return currentDc;
}

int addNumbersData(int* dataArray, int arrayLength){
    int i = 0, currentDc = dc;
    for(i = 0;i< arrayLength; i++){
        dataCollection[dc] = dataArray[i];
        dc++;
    }    
    return currentDc;
}

int getDataByID(int dcRefrenceID){
    return dataCollection[dcRefrenceID];    
}

void clearData(){
    int i = 0;
    for(i = 0;i < dc; i++){
        dataCollection[i] = 0;
    }    
    dc = 0;
}

int getDCPointer(){
    return dc;
}

void printDataCollection(int icRefrence){
    int i = 0, lineRefrence;
    char *lineNumber, *dataOutput;
    WordDef * base8;

    for(i = 0;i < dc; i++){
        lineRefrence = i + icRefrence;
        lineNumber = getSpecialBase8String(lineRefrence);
        base8 = convertDecimalToBase8(dataCollection[i]);
        dataOutput = getBase8String(base8);
        
        printf("%s  %s\n",lineNumber,dataOutput);
        objWriteToFile("%s  %s\n", lineNumber, dataOutput);

        free(lineNumber);
        free(base8);
        free(dataOutput);
    }    
}