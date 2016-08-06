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
    for(i = 0; i < length;i++)
    {
        dataCollection[dc] = data[i];
        dc++;
    }
    /* add the indicator for the end of the string */
    dataCollection[dc] = '\0';
    dc++;
    /* return the dc of the beginning of the string */
    return currentDc;
}

int addNumbersData(int* dataArray, int arrayLength)
{
    int i = 0, currentDc = dc;
    for(i = 0;i< arrayLength; i++)
    {
        dataCollection[dc] = dataArray[i];
        dc++;
    }    
    /* return the dc of the first number */
    return currentDc;
}

int getDataByID(int dcRefrenceID)
{
    return dataCollection[dcRefrenceID];    
}

void clearData()
{
    int i = 0;
    for(i = 0;i < dc; i++)
    {
        /* set all the values to zero */
        dataCollection[i] = 0;
    }    
    dc = 0;
}

int getDCPointer()
{
    return dc;
}

void printDataCollection(int icRefrence)
{
    int i = 0, lineRefrence;
    char *lineNumber, *dataOutput;
    WordDef * base8;

    for(i = 0;i < dc; i++)
    {
        lineRefrence = i + icRefrence;
        /* convert the data and line numbers to special base 8*/
        lineNumber = getSpecialBase8String(lineRefrence);
        base8 = convertDecimalNumberToBase8Word(dataCollection[i]);
        dataOutput = getStringFromBase8Word(base8);
        /* print to the ob file*/
        objWriteToFile("%s  %s\n", lineNumber, dataOutput);

        free(lineNumber);
        free(base8);
        free(dataOutput);
    }    
}