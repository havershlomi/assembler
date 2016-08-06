#include "utils.h"

static int hasError = false;

void printErr(char * format,...)
{
    va_list arg;
    hasError = true;
    va_start (arg, format);
    vfprintf (stderr, format, arg);
    va_end (arg); 
}

int hasErrorInInput()
{
    return hasError;
}

int cutByBits(const int number, int from, int to)
{
    int i = 0, result = number, rangeSize = to - from;
    char bitSign = '0';

    /*cut all the bits until from*/
    result >>= from;
    /* check the status of the bit at to index*/
    bitSign = ((result >> rangeSize) & 1) +'0';
    
    /*flips all the other bits to 1 or 0 depends on bitSign*/
    for (i = rangeSize ; i < 15; i++) 
    {
        if(bitSign == '1')
            result = result | 1 << i ;
        else
            result = result & ~(1 << i) ;
    }
    
    return result;
}

void resetErrorFlag()
{
    hasError =  false;
}