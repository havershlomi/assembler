#include "dataCollection.h"
int dc = 0;
int dataCollection[1000];


int addStringData(char* data)
{
    int i = 0, length = strlen(data);
    for(i = 0; i < length;i++){
        dataCollection[dc] = data[i];
        dc++;
    }
    dataCollection[dc] = '\0';
    dc++;
    return dc;
}

int addNumbersData(int* dataArray, int arrayLength){
    int i = 0;
    for(i = 0;i< arrayLength; i++){
        dataCollection[dc] = dataArray[i];
        dc++;
    }    
    return dc;
}

void clearData(){

}