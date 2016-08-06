#include "dataStructure.h"
#include "binaryConvertor.h"
#include "utils.h"

/* special base 8 definition */
static char specialOctalBase[8] = {'!','@','#','$','%','^','&','*'};


unsigned int convertCommandWordToInt(WordDef *wordDef)
{
    int result = 0;

    result = result | (wordDef -> command.ERA);
    result = result | (wordDef -> command.dest << 2);
    result = result | (wordDef -> command.src << 4);
    result = result | (wordDef -> command.opCode << 6);
    result = result | (wordDef -> command.group << 10);
    result = result | (wordDef -> command.notUsed << 12);

    return result;
}

unsigned int convertRegisterValueWordToInt(WordDef *wordDef)
{
    int result = 0;

    result = result | (wordDef -> registerValue.ERA);
    result = result | (wordDef -> registerValue.dest << 2);
    result = result | (wordDef -> registerValue.src << 8);
    result = result | (wordDef -> registerValue.notUsed << 14);
    
    return result;
}

unsigned int convertRegularValueWordToInt(WordDef *wordDef)
{
    int result = 0;

    result = result | (wordDef -> regularValue.ERA);
    result = result | (wordDef -> regularValue.value << 2);
    return result;
}

WordDef * convertDecimalNumberToBase8Word(int number)
{
    WordDef * base8 = (WordDef*)malloc(sizeof(WordDef));

    if(base8)
    {
        base8 -> Base8.block1 = number;
        base8 -> Base8.block2 = number >> 3;
        base8 -> Base8.block3 = number >> 6;
        base8 -> Base8.block4 = number >> 9;
        base8 -> Base8.block5 = number >> 12;        
    } 
    else 
    {
        printf("can't allocate space for Base8 word\n");
    }

    return base8;
}

char* getStringFromBase8Word(WordDef * base8)
{
    char *output = (char*)malloc(OCTAL_WORD_SIZE);

    if(base8)
    {
        /* get the sign for each block */
        output[4] = specialOctalBase[base8 -> Base8.block1];
        output[3] = specialOctalBase[base8 -> Base8.block2];
        output[2] = specialOctalBase[base8 -> Base8.block3];
        output[1] = specialOctalBase[base8 -> Base8.block4];
        output[0] = specialOctalBase[base8 -> Base8.block5];
        output[5] = '\0';
    } 
    else 
    {
        printf("can't allocate space for output\n");
    }
    return output;
}

char* getSpecialBase8String(int decimalNumber)
{
    int octalNumber[WORD_SIZE],i = 1 , j, index = 0;
    long int quotient;
    char *output = (char *)malloc(WORD_SIZE);

    if(output)
    {
        quotient = decimalNumber;
        /* devide by 8 until and save the remainer */
        while(quotient != 0)
        {
            octalNumber[i++] = quotient % 8;
            quotient = quotient / 8;
        }
        /* add the sign for each remainer */
        for(j = i -1 ;j > 0;j--)
        {
            output[index++] = (char)specialOctalBase[octalNumber[j]]; 
        }
        output[index++] = '\0';
    } 
    else 
    {
        printf("can't allocate space for output\n");
    }

    return output;
}