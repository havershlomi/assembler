#include "dataCollection.h"
static int dc = 0;
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