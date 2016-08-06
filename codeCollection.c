#include "codeCollection.h"
#include "dataCollection.h"
#include "binaryConvertor.h"
#include "utils.h"

extern Action ValidActions[];
static int ic = 0;
static Word codeCollection[1000];
int getActionAddressingType(char *action,char *actionAttr){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int srcAddressingType = 0, destAddressingType = 0, icPointer = INVALID;

    /*find action*/
    selectedAction = getActionByName(action);
    if(selectedAction != NULL){
        /*handle action with 2 operands*/
        if(selectedAction -> numOfOperands == 2){
            if(sscanf(actionAttr," %[^ \r\t:,] , %[^ \r\t:] %[^\n] ",firstOper,secondOper,extraData) >= 2){
                if(strlen(extraData) != 0){
                    printErr("%s can not get more than 2 operands\n",action);
                } else {
                    srcAddressingType = getOperandType(firstOper);
                    destAddressingType = getOperandType(secondOper);

                    if(isValidAddressTypeForAction(srcAddressingType,selectedAction -> sourceOper) == true
                        && isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == true){
                        icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                                        srcAddressingType,destAddressingType);                        
                        
                        getActionRefrenceinMemory(srcAddressingType, destAddressingType);  
                    } else {
                        printErr("invalid block address method for action '%s'\n'",action);                
                    }    
                }
            } else {
                printErr("%s except to get 2 operands\n",action);                
            }
            /*handle action with 1 operands*/
        } else if(selectedAction -> numOfOperands == 1){
            destAddressingType = getOperandType(actionAttr);
            if(isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == false){
                printErr("invalid block address method for action '%s' \n'",action);
            } else {
                icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                0,destAddressingType);
                getActionRefrenceinMemory(notUsedOper, destAddressingType);
            }   
        }
        else {
            /*handle actions that have zero operands*/
            printf("%s",actionAttr);
            if(actionAttr != NULL && sscanf(actionAttr," %[^\n] ",extraData) == 1){
                printErr(action);
                printErr(" can not accept operands\n");               
            } else {
                
                icPointer = addActionWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,0,0);
                getActionRefrenceinMemory(notUsedOper, notUsedOper);
            }
        }
    }
    else {
        printErr("the action name is invalid\n");
    }
    return icPointer;
}

int addActionWord(int actionIndex, int numOfOperands,int srcAddressingType,int destAddressingType){
    int icPointer = INVALID;
    WordDef * wordDef;

    wordDef = createCommandWord(actionIndex, numOfOperands,
                srcAddressingType,destAddressingType,absolute);
    if(wordDef){
        icPointer = addToCollection(wordDef,commandType);
    }

    return icPointer;
}


int addActionToCodeCollection(char *action,char *actionAttr){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int srcAddressingType, destAddressingType = -1;
    WordDef *wordDef;

    /*find action*/
    selectedAction = getActionByName(action);
    if(selectedAction != NULL){
        /*handle action with 2 operands*/
        if(selectedAction -> numOfOperands == 2){
            if(sscanf(actionAttr," %[^ \r\t:,] , %[^ \r\t:] %[^\n] ",firstOper,secondOper,extraData) >= 2){
                if(strlen(extraData) != 0){
                    printErr("%s can not get more than 2 operands\n",action);
                } else {
                    srcAddressingType = getOperandType(firstOper);                    
                    destAddressingType = getOperandType(secondOper);
                    if(isValidAddressTypeForAction(srcAddressingType,selectedAction -> sourceOper) == true
                        && isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == true){
                        
                        if((srcAddressingType == directRegister && destAddressingType == directRegister) || 
                            (srcAddressingType == directRegister || destAddressingType == directRegister)){
                            wordDef = createRegisterWord(srcAddressingType,firstOper, destAddressingType, secondOper);
                            addToCollection(wordDef, registerValueType);
                        }
                        
                        addDataWordToCollection(srcAddressingType, firstOper);
                        addDataWordToCollection(destAddressingType, secondOper); 

                    } else {
                        printErr("invalid block address method for action '%s'\n'",action);                
                    }    
                }
            } else {
                printErr("%s except to get 2 operands\n",action);                
            }
            /*handle action with 1 operands*/
        } else if(selectedAction -> numOfOperands == 1){
            destAddressingType = getOperandType(actionAttr);
            if(isValidAddressTypeForAction(destAddressingType,selectedAction -> destOper) == false){
                printErr("invalid block address method for action '%s' \n'",action);
            } else {
                
                if(destAddressingType == directRegister){
                    wordDef = createRegisterWord(notUsedOper, NULL, destAddressingType, actionAttr);
                    addToCollection(wordDef, registerValueType);
                }else{
                    addDataWordToCollection(destAddressingType, actionAttr);
                }                
            }   
        }
        else {
            /*handle actions that have zero operands*/
            if(actionAttr != NULL && sscanf(actionAttr," %[^\n] ",extraData) == 1){
                printErr("%s can not accept operands\n",action);               
            }
        }
    }
    else {
        printErr("the action name is invalid\n");
    }
    return INVALID;
}
int addToCollection(WordDef * wordDef, int wordType){
    int currentIc;
    /*find the first slot open*/
    while(codeCollection[ic].word){
        ic++;
    }
    currentIc = ic;
    codeCollection[ic].word = wordDef;
    codeCollection[ic].wordType = wordType;
    ic++;
    return currentIc + IC_START_POSITION;
}

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era){
    WordDef* wordDef = (WordDef*)malloc(sizeof(WordDef));

    if(wordDef != NULL){
        wordDef -> command.notUsed = NOT_USED_DEFAULT_VALUE;
        wordDef -> command.opCode = opCode;
        wordDef -> command.group = group;
        wordDef -> command.dest = dest;
        wordDef -> command.src = src;
        wordDef -> command.ERA = Era;
        return wordDef;
    }
    printErr("can not create a command Word\n");
    return NULL;
}

void addDataWordToCollection(int addressingType, char* value){
    WordDef * wordDef;
    int addedAtRow = -1, dataType = -1;
    if(addressingType == directRegister){
        return;
    }
    else if(addressingType == instant){
        wordDef = createInstantWord(value);
        dataType = regularValueType;
    } else if(addressingType == direct){
        wordDef = createDirectWord(value);
        dataType = regularValueType;
    } else if(addressingType == dynamic){
        wordDef = createDynamicWord(value);
        dataType = regularValueType;        
    }
    
    if(wordDef){
        addedAtRow = addToCollection(wordDef, dataType);
        if(wordDef -> regularValue.ERA == externalData){
            externalWriteToFile("%s %d\n",value,addedAtRow);
        }
    }
}

/* get data word */
/* createDirectWord get a word defining an by symbol

*/
WordDef* createDynamicWord(char* value){
    WordDef * word;
    Symbol * symbol;
    char symbolName[LINE_LENGTH] = "";
    int fromBit = -1, toBit = -1, refrence = -1, data = 0;
    if(sscanf(value,"%[^[][%d-%d]",symbolName,&fromBit,&toBit) == 3 
        && fromBit < toBit && fromBit >= 0 && toBit <= 13){
        
        symbol = getSymbolByName(symbolName);
        if(!symbol){
            printErr("%s is never defined.\n",value);
        } else {

            word = (WordDef*)malloc(sizeof(WordDef));
            if(word){

                refrence = symbol -> icRefrence;
                if(symbol -> commandType == instructionCommand){
                    refrence = symbol -> dcRefrence;
                    data = getDataByID(refrence);                    
                } else if(symbol -> commandType == actionCommand){
                    data = getWordNumberByRefrenceID(refrence);
                }
                word -> regularValue.value = cutByBits(data, fromBit, toBit);                
                word -> regularValue.ERA = absolute;
                
            } else {
                printErr("can't allocate space for new word \n");
            }
        }
    } else {
        printErr("%s is an invalid operandor\n",value);
    }
    
    return word;
}

/* get ic refrence and return the number that represents the command word */
unsigned int getWordNumberByRefrenceID(int refrenceID){
    unsigned int result = 0;
    Word * word;
    WordDef *  wordDef;
    word = &codeCollection[refrenceID - IC_START_POSITION];
    if(!word)
        printErr("there is no word in this index: %d\n",refrenceID);
    else {
        wordDef = word -> word;
        if(wordDef){
            /*shift bits to get the number they represent*/
           result = convertCommandWordToInt(wordDef);
        }
    }
    return result;
}


/* get data word */
/* createDirectWord get a word defining an by symbol

*/
WordDef* createDirectWord(char* value){
    WordDef * word;
    Symbol * symbol;
    symbol = getSymbolByName(value);
    if(symbol != NULL){
        word = (WordDef*)malloc(sizeof(WordDef));
        if(word != NULL){
            word -> regularValue.value = symbol -> icRefrence;
            word -> regularValue.ERA = relocatable;
            if((symbol -> isExternal) == true)
            {
                word -> regularValue.ERA = externalData;
            }
        } else {
            printErr("can't allocate space for new word \n");
        }
    } else {
        printErr("%s is never defined.\n",value);
    }

    return word;
}

/* get data word */
/* createInstantWord get a word defining an instatnt word

*/
WordDef* createInstantWord(char* value){
    WordDef * word;    
    int number = 0;
    char sign = '+';
    word = (WordDef*)malloc(sizeof(WordDef));
    if(word != NULL){
        word -> regularValue.ERA = absolute;
        
        if(sscanf(value,"#%[-+]%d",&sign,&number) == 2 || sscanf(value,"#%d",&number) == 1){
            if(sign == '-'){
                number *= -1;
            }
            word -> regularValue.value = number;
        }
    } else {
        printErr("can't allocate space for new word \n");
    }

    return word;
}

WordDef* createRegisterWord(int srcAddressType, char* srcWord, int destAdressType, char* destWord){
    WordDef * word;    
    int srcNumber = 0, destNumber = 0;
    
    word = (WordDef*)malloc(sizeof(WordDef));
    if(word != NULL){
        word -> registerValue.notUsed = 0;
        word -> registerValue.ERA = absolute;
        
        if(destAdressType == directRegister && sscanf(destWord,"r%d",&destNumber) == 1){
            word -> registerValue.dest = destNumber;
        }
        if(srcAddressType == directRegister && sscanf(srcWord,"r%d",&srcNumber) == 1){
            word -> registerValue.src = srcNumber;
        }
    } else {
        printErr("can't allocate space for new word \n");
    }

    return word;
}

Action* getActionByName(const char* name){
    int i = 0;
    for(i = 0; i < 16; i++){
        if(strcmp((ValidActions[i].name),name) == 0){
            return &ValidActions[i];
        }
    }
    return NULL;
}

int getOperandType(const char* oper){
    int number = -1, number2 = -1, operandType = invalidOperand;
    char operAttr[LINE_LENGTH]  = "", extraData[LINE_LENGTH] = "";
    
    if(sscanf(oper," #%*[-+]%d %[^\n]",&number,extraData) >= 1 ||
        sscanf(oper," #%d %[^\n]",&number,extraData) >= 1){
        if(strlen(extraData) == 0 && number != -1)
            operandType = instant;
    } else if(sscanf(oper," %*[r]%d %[^\n]",&number,extraData) >= 1){
        if(number >= 0 && number <= 7 && strlen(extraData) == 0)
            operandType = directRegister;
    } else if(sscanf(oper," %*[^[][%d-%d] %[^\n]",&number,&number2,extraData) >= 2 ||
            sscanf(oper," %*[^[][%d-] %[^\n]",&number,extraData) >= 1 ||
            sscanf(oper," %*[^[][-%d] %[^\n]",&number,extraData) >= 1){
        if(strlen(extraData) == 0 && number < number2 && number >= 0 && number2 <= 13)
            operandType = dynamic;
    } else if(sscanf(oper," %[^ \t\r] %[^\n]",operAttr,extraData) >= 1 && strchr(oper,'[') == NULL
             && strchr(oper,']') == NULL && strlen(extraData) == 0){
            operandType = direct;
    } 
    return operandType;
}

int isValidAddressTypeForAction(int sourcingType, int* validBLA){
    int i = 0, arrayMaxLength = 4; 
    for(i = 0; i < arrayMaxLength; i++){
        if(validBLA[i] == -1)
            return false;
        if(validBLA[i] == sourcingType){
            return true;
        }
    }
    return false;
}

int getActionRefrenceinMemory(int blaSrc, int blaDest){
    int numberOfRows = 0, currentIc = ic;
    if(blaDest == directRegister && blaSrc == directRegister){
        numberOfRows += 1;
    } else if(blaSrc != notUsedOper && blaDest != notUsedOper){
        numberOfRows += 2;
    } 
    else if((blaSrc == notUsedOper && blaDest != notUsedOper) ||
            (blaDest == notUsedOper && blaSrc != notUsedOper)){
        numberOfRows += 1;
    } 
    ic += numberOfRows;

    return currentIc + IC_START_POSITION;
}

int getICPointer(){
    return ic + IC_START_POSITION;
}

void resetIc(){
    ic = 0;
}


void printCodeCollection(){
    Word *word;
    WordDef * base8;
    int i = 0, wordAsInt = 0;
    char *output, *addressOutput;
    
    objWriteToFile("Base 8 Special Address  |   Base 8 special code\n");
    objWriteToFile("print the size of both collection here\n");
    for(i = 0; i < ic; i++)
    {
        word = &codeCollection[i];
        if(word != NULL && word -> word != NULL){
            if(word -> wordType == commandType){
                /*printf("%d|%d|%d|%d|%d|%d\n",word -> word -> command.notUsed,word -> word -> command.group,word -> word -> command.opCode,
                word -> word -> command.src,word -> word -> command.dest,word -> word -> command.ERA);*/
                wordAsInt = convertCommandWordToInt(word -> word);
            }  else if(word -> wordType == registerValueType){
                /*printf(" %d | %d | %d | %d \n",word -> word -> registerValue.notUsed, word -> word -> registerValue.src,
                word -> word -> registerValue.dest,word -> word -> registerValue.ERA);*/
                wordAsInt = convertRegisterValueWordToInt(word -> word);
                
            }  else if(word -> wordType == regularValueType){
                /*printf(" %d | %d \n",word -> word -> regularValue.value, word -> word -> regularValue.ERA);*/
                wordAsInt = convertRegularValueWordToInt(word -> word);
            }
            base8  = convertToBase8(wordAsInt);
            output = getBase8String(base8);
            addressOutput = getSpecialBase8String(i+IC_START_POSITION);

            printf("%s  %s\n",addressOutput,output);
            objWriteToFile("%s  %s\n",addressOutput,output);
            
            free(output);
            free(addressOutput);
        }
    }
}
