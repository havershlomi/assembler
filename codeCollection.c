#include "codeCollection.h"
#include "dataCollection.h"
#include "binaryConvertor.h"
#include "utils.h"

/* get this from the sharedCollections file*/
extern Action ValidActions[];
/* ic counter */
static int ic = 0;
/* code collection */
static Word codeCollection[1000];
/* an empty word to reset the array with */
static const Word EmptyWord;

int calculateActionIcPointer(char *action,char *actionAttr)
{
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int srcAddressingType = 0, destAddressingType = 0, icPointer = INVALID;

    /*find action*/
    selectedAction = getActionByName(action);
    if(selectedAction != NULL)
    {
        /*handle action with 2 operands*/
        if(selectedAction -> numOfOperands == 2)
        {
            /* get the operands */            
            if(sscanf(actionAttr," %[^ \r\t:,] , %[^ \r\t:,] %[^\n] ",firstOper,secondOper,extraData) >= 2)
            {
                if(strlen(extraData) != 0)
                {
                    printErr("'%s' can not get more than 2 operands\n",action);
                } 
                else
                {
                    srcAddressingType = getOperandType(firstOper);
                    destAddressingType = getOperandType(secondOper);
                    /* check if the opearnads are valid for this action */
                    if(isValidAddressTypeForAction(srcAddressingType,selectedAction -> sourceOper) == true
                        && isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == true)
                    {
                        /* add the action word to the colleciton */
                        icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                                        srcAddressingType,destAddressingType);                        
                        /* check how muc space is needed for the data word of this action */
                        getActionRefrenceinMemory(srcAddressingType, destAddressingType);  
                    } 
                    else
                    {
                        printErr("invalid addressing method for action '%s'\n",action);                
                    }    
                }
            } 
            else 
            {
                printErr("%s expect to get 2 operands\n",action);                
            }
        } 
        else if(selectedAction -> numOfOperands == 1)
        {
            /*handle action with 1 operands*/
            destAddressingType = getOperandType(actionAttr);
            if(isValidAddressTypeForAction(destAddressingType, selectedAction -> destOper) == false)
            {
                printErr("invalid addressing method for action '%s' \n",action);
            } 
            else
            {
                /* add the action word to the colleciton */
                icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                0,destAddressingType);
                /* check how much space is needed for the data word of this action */
                getActionRefrenceinMemory(notUsedOper, destAddressingType);
            }   
        }
        else 
        {
            /*handle actions that have zero operands*/
            if(actionAttr != NULL && sscanf(actionAttr," %[^\n] ",extraData) == 1)
            {
                printErr("'%s' can not accept operands\n",action);               
            } 
            else 
            {            
                /* add action word */
                icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,0,0);
                /* check how much space is needed for the data word of this action */
                getActionRefrenceinMemory(notUsedOper, notUsedOper);
            }
        }
    }
    else 
    {
        printErr("'%s' is an invalid action name\n",action);
    }

    return icPointer;
}

int addActionWord(int actionIndex, int numOfOperands,int srcAddressingType,int destAddressingType)
{
    int icPointer = INVALID;
    WordDef * wordDef;

    wordDef = createCommandWord(actionIndex, numOfOperands,
                srcAddressingType,destAddressingType,absolute);

    if(wordDef)
    {
        icPointer = addToCollection(wordDef,commandType);
    }

    return icPointer;
}


void addActionToCodeCollection(char *action,char *actionAttr){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int srcAddressingType, destAddressingType = -1;
    WordDef *wordDef;

    /*find action*/
    selectedAction = getActionByName(action);
    if(selectedAction != NULL)
    {
        /*handle action with 2 operands*/
        if(selectedAction -> numOfOperands == 2)
        {
            /* get the operands */
            if(sscanf(actionAttr," %[^ \r\t:,] , %[^ \r\t:,] %[^\n] ",firstOper,secondOper,extraData) >= 2)
            {
                if(strlen(extraData) == 0) 
                {
                    srcAddressingType = getOperandType(firstOper);                    
                    destAddressingType = getOperandType(secondOper);

                    /* check if both operands are valid fot his action type */
                    if(isValidAddressTypeForAction(srcAddressingType,selectedAction -> sourceOper) == true
                        && isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == true)
                    {
                        if(srcAddressingType != directRegister)
                            addDataWordToCollection(srcAddressingType, firstOper);

                        /* if one of the operands is a register type add it to the collection */    
                        if((srcAddressingType == directRegister && destAddressingType == directRegister) || 
                            (srcAddressingType == directRegister || destAddressingType == directRegister))
                        {
                            wordDef = createRegisterWord(srcAddressingType,firstOper, destAddressingType, secondOper);
                            addToCollection(wordDef, registerValueType);
                        }

                        if(destAddressingType != directRegister)
                            addDataWordToCollection(destAddressingType, secondOper);
                    }
                }
            }
        } 
        else if(selectedAction -> numOfOperands == 1)
        {
            /*handle action with 1 operands*/
            destAddressingType = getOperandType(actionAttr);

            if(isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == true) 
            {    
                /* check the operand type and add it to the collection */
                if(destAddressingType == directRegister)
                {
                    wordDef = createRegisterWord(notUsedOper, NULL, destAddressingType, actionAttr);
                    addToCollection(wordDef, registerValueType);
                }
                else
                {
                    addDataWordToCollection(destAddressingType, actionAttr);
                }                
            }   
        }
    }
}

int addToCollection(WordDef * wordDef, int wordType)
{
    int currentIc;
    /*find the first slot open*/
    while(codeCollection[ic].word)
    {
        ic++;
    }
    currentIc = ic;

    /* add the word to this space */
    codeCollection[ic].word = wordDef;
    codeCollection[ic].wordType = wordType;
    ic++;

    /* return the ic pointer for this word */
    return currentIc + IC_START_POSITION;
}

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era)
{
    WordDef* wordDef = (WordDef*)malloc(sizeof(WordDef));

    if(wordDef != NULL)
    {
        wordDef -> command.notUsed = NOT_USED_DEFAULT_VALUE;
        wordDef -> command.opCode = opCode;
        wordDef -> command.group = group;
        wordDef -> command.dest = dest;
        wordDef -> command.src = src;
        wordDef -> command.ERA = Era;
        return wordDef;
    }
    printf("can not allocate space for a command Word\n");
    return NULL;
}

void addDataWordToCollection(int addressingType, char* value)
{
    WordDef * wordDef;
    int addedAtRow = -1, dataType = -1;
    char *lineNumber;

    /* find the right word type and create a word object */
    if(addressingType == instant)
    {
        wordDef = createInstantWord(value);
        dataType = regularValueType;
    } 
    else if(addressingType == direct)
    {
        wordDef = createDirectWord(value);
        dataType = regularValueType;
    } 
    else if(addressingType == dynamic)
    {
        wordDef = createDynamicWord(value);
        dataType = regularValueType;        
    }
    
    if(wordDef)
    {
        /* add word to the code collection */
        addedAtRow = addToCollection(wordDef, dataType);
        if(wordDef -> regularValue.ERA == externalData)
        {
            /* write in external file */
            lineNumber = getSpecialBase8String(addedAtRow);
            externalWriteToFile("%s %s\n",value,lineNumber);
            free(lineNumber);
        }
    } 
}

WordDef* createDynamicWord(char* value)
{
    /* data pointers  */
    WordDef * word;
    Symbol * symbol;
    char symbolName[LINE_LENGTH] = "";
    int fromBit = -1, toBit = -1, refrence = -1, data = 0;

    /* get the label and the two numbers specifing the range */
    if(sscanf(value,"%[^[][%d-%d]",symbolName,&fromBit,&toBit) == 3 
        && fromBit < toBit && fromBit >= 0 && toBit <= 13)
    {
        /* get the symbol by his name */    
        symbol = getSymbolByName(symbolName);
        if(!symbol)
        {
            printErr("%s is never defined.\n",value);
        }
        else
        {
            word = (WordDef*)malloc(sizeof(WordDef));
            if(word)
            {
                refrence = symbol -> icRefrence;
                /* if instruction get the data from the data collection */
                if(symbol -> commandType == instructionCommand)
                {
                    refrence = symbol -> dcRefrence;
                    data = getDataByID(refrence);                    
                } 
                else if(symbol -> commandType == actionCommand)
                {
                    /* if command get the command ic position */
                    data = getCommandICPositionByRefrenceID(refrence);
                }
                /* get the new value for the specified range */                
                word -> regularValue.value = cutByBits(data, fromBit, toBit);                
                word -> regularValue.ERA = absolute;
            }
            else
            {
                printf("can't allocate space for new word \n");
            }
        }
    } 
    else 
    {
        printErr("%s is an invalid operandor\n",value);
    }
    
    return word;
}

/* get ic refrence and return the number that represents the command word */
unsigned int getCommandICPositionByRefrenceID(int refrenceID)
{
    unsigned int result = 0;
    Word * word;
    WordDef *  wordDef;
    word = &codeCollection[refrenceID - IC_START_POSITION];
    
    if(!word)
        printf("there is no word in this index: %d\n",refrenceID);
    else 
    {
        wordDef = word -> word;
        if(wordDef)
        {
            /*shift bits to get the number they represent*/
           result = convertCommandWordToInt(wordDef);
        }
    }
    return result;
}



WordDef* createDirectWord(char* value){
    WordDef * word;
    Symbol * symbol;
    symbol = getSymbolByName(value);

    if(symbol != NULL)
    {
        word = (WordDef*)malloc(sizeof(WordDef));
        if(word != NULL)
        {
            word -> regularValue.value = symbol -> icRefrence;
            word -> regularValue.ERA = relocatable;
            if((symbol -> isExternal) == true)
            {
                word -> regularValue.ERA = externalData;
            }
        } 
        else 
        {
            printf("can't allocate space for new word \n");
        }
    } 
    else 
    {
        printErr("'%s' is never defined.\n",value);
    }

    return word;
}

/* get data word */

WordDef* createInstantWord(char* value)
{
    WordDef * word;    
    int number = 0;
    char sign = '+';
    word = (WordDef*)malloc(sizeof(WordDef));
    if(word != NULL)
    {
        word -> regularValue.ERA = absolute;
        
        if(sscanf(value,"#%[-+]%d",&sign,&number) == 2 || sscanf(value,"#%d",&number) == 1)
        {
            if(sign == '-')
            {
                number *= -1;
            }
            word -> regularValue.value = number;
        }
    } 
    else 
    {
        printErr("can't allocate space for new instant word \n");
    }

    return word;
}

WordDef* createRegisterWord(int srcAddressType, char* srcWord, int destAdressType, char* destWord)
{
    WordDef * word;    
    int srcNumber = 0, destNumber = 0;
    
    word = (WordDef*)malloc(sizeof(WordDef));

    if(word != NULL)
    {
        word -> registerValue.notUsed = 0;
        word -> registerValue.ERA = absolute;
        
        if(destAdressType == directRegister && sscanf(destWord,"r%d",&destNumber) == 1)
        {
            word -> registerValue.dest = destNumber;
        }
        if(srcAddressType == directRegister && sscanf(srcWord,"r%d",&srcNumber) == 1)
        {
            word -> registerValue.src = srcNumber;
        }
    } 
    else 
    {
        printErr("can't allocate space for new word \n");
    }

    return word;
}

Action* getActionByName(const char* name)
{
    int i = 0;
    for(i = 0; i < 16; i++)
    {
        if(strcmp((ValidActions[i].name),name) == 0)
        {
            return &ValidActions[i];
        }
    }
    return NULL;
}

int getOperandType(const char* oper)
{
    int number = -1, number2 = -1, operandType = invalidOperand;
    /* char arrays for sscanf */
    char operAttr[LINE_LENGTH]  = "", extraDataInstent[LINE_LENGTH] = "",
     extraDataDirect[LINE_LENGTH] = "", extraDataDynamic[LINE_LENGTH] = "", extraDataRegister[LINE_LENGTH] = "";
    
    if(sscanf(oper," #%*[-+]%d %[^\n]",&number,extraDataInstent) >= 1 ||
        sscanf(oper," #%d %[^\n]",&number,extraDataInstent) >= 1)
    {
        /* check if the instant operans is valid */
        if(strlen(extraDataInstent) == 0 && number != -1)
            operandType = instant;
    } 
    else if(sscanf(oper," %*[r]%d %[^\n]",&number,extraDataRegister) >= 1 && strlen(extraDataRegister) == 0)
    {
        /* check if the register operand is valid */
        if(number >= 0 && number <= 7)
            operandType = directRegister;
        else 
            operandType = direct;
    } 
    else if(sscanf(oper," %*[^[][%d-%d] %[^\n]",&number,&number2,extraDataDynamic) >= 2 ||
            sscanf(oper," %*[^[][%d-] %[^\n]",&number,extraDataDynamic) >= 1 ||
            sscanf(oper," %*[^[][-%d] %[^\n]",&number,extraDataDynamic) >= 1)
    {
        /*  check if the dynamic operand is valid */
        if(strlen(extraDataDynamic) == 0 && number < number2 && number >= 0 && number2 <= 13)
            operandType = dynamic;
    } 
    else if(sscanf(oper," %[^ \t\r] %[^\n]",operAttr,extraDataDirect) >= 1 && strchr(oper,'[') == NULL
             && strchr(oper,']') == NULL && strlen(extraDataDirect) == 0)
    {
        operandType = direct;
    }
    return operandType;
}

int isValidAddressTypeForAction(int sourcingType, int* validAddressingTypes)
{
    int i = 0, arrayMaxLength = 4; 
    for(i = 0; i < arrayMaxLength; i++)
    {
        if(validAddressingTypes[i] == -1)
            return false;

        if(validAddressingTypes[i] == sourcingType)
            return true;
        
    }
    return false;
}

int getActionRefrenceinMemory(int srcAddressingType, int destAddressingType)
{
    int numberOfRows = 0, currentIc = ic;

    if(destAddressingType == directRegister && srcAddressingType == directRegister)
    {
        numberOfRows += 1;
    } 
    else if(srcAddressingType != notUsedOper && destAddressingType != notUsedOper)
    {
        numberOfRows += 2;
    } 
    else if((srcAddressingType == notUsedOper && destAddressingType != notUsedOper) ||
            (destAddressingType == notUsedOper && srcAddressingType != notUsedOper))
    {
        numberOfRows += 1;
    }

    ic += numberOfRows;

    return currentIc + IC_START_POSITION;
}

int getICPointer()
{
    return ic + IC_START_POSITION;
}

int getICCollectionSize()
{
    return ic;
}

void resetIc()
{
    ic = 0;
}


void printCodeCollection(){
    Word *word;
    WordDef * base8;
    int i = 0, wordAsInt = 0;
    char *output = "", *addressOutput = "";
    
    for(i = 0; i < ic; i++)
    {
        word = &codeCollection[i];
        if(word != NULL && word -> word != NULL){
            if(word -> wordType == commandType)
            {
                wordAsInt = convertCommandWordToInt(word -> word);
            }
            else if(word -> wordType == registerValueType)
            {
                wordAsInt = convertRegisterValueWordToInt(word -> word);    
            } 
            else if(word -> wordType == regularValueType)
            {
                wordAsInt = convertRegularValueWordToInt(word -> word);
            }

            base8  = convertDecimalNumberToBase8Word(wordAsInt);
            output = getStringFromBase8Word(base8);
            addressOutput = getSpecialBase8String(i+IC_START_POSITION);

            objWriteToFile("%s  %s\n",addressOutput,output);

            free(base8);
            free(output);
            free(addressOutput);
        }
    }
}

void cleanCodeCollection()
{
     int i = 0;
    for(i = 0;i < ic; i++)
    {
        codeCollection[i] = EmptyWord;
    }    
    
}