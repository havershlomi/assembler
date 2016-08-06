#include <stdio.h>
#include <string.h>

/* add each char of the string into the data collection 
return the pointer value for the first char
*/
int addStringData(char* data);

/* add each number to the data collection 
return the pointerULL; value for the first number
*/
int addNumbersData(int* data, int arrayLength);

/* get date value by pointer
return the value in the specific pointer*/
int getDataByID(int refrenceID);

/* clear all the data from the data collection*/
void clearData();

/* get the current dc pointer */
int getDCPointer();

/* print the entire data collection
get the ic last position to indicate where the data collection area starts */
void printDataCollection(int icRefrence);
