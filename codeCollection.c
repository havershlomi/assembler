#include "codeCollection.h"
#include "utils.h"

extern Action ValidActions[];
static int ic = 0;
static Word codeCollection[1000];

int addActionToCodeCollection(char *action,char *actionAttr){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int blockAddressSourceType = -1, blockAddressDestType = -1;
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
                    blockAddressSourceType = getOperandType(firstOper);
                    blockAddressDestType = getOperandType(secondOper);

                    if(isValidBlockAddressTypeForAction(blockAddressSourceType,selectedAction -> sourceOper) == true
                        && isValidBlockAddressTypeForAction(blockAddressDestType,selectedAction -> destOper) == true){
                        /* return getActionRefrenceinMemory(blockAddressSourceType, blockAddressDestType);*/ 
                    } else {
                        printErr("invalid block address method for action '%s'\n'",action);                
                    }    
                }
            } else {
                printErr("%s except to get 2 operands\n",action);                
            }
            /*handle action with 1 operands*/
        } else if(selectedAction -> numOfOperands == 1){
            blockAddressDestType = getOperandType(actionAttr);
            if(isValidBlockAddressTypeForAction(blockAddressDestType,selectedAction -> destOper) == false){
                printErr("invalid block address method for action '%s' \n'",action);
            } else {
            printf("%d",blockAddressDestType);
                /*return getActionRefrenceinMemory(notUsedOper, blockAddressDestType);*/
                wordDef = createCommandWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                0,blockAddressDestType,absolute);
                /* chack which ERA comes here check if need to write to extern file*/
                addToCollection(wordDef,commandType);
            }   
        }
        else {
            /*handle actions that have zero operands*/
            if(actionAttr != NULL && sscanf(actionAttr," %[^\n] ",extraData) == 1){
                printErr("%s can not accept operands\n",action);               
            } else {
                wordDef = createCommandWord(selectedAction -> binaryIndex, selectedAction -> numOfOperands,
                0,0,absolute);
                addToCollection(wordDef,commandType);
            }
        }
    }
    else {
        printErr("the action name is invalid\n");
    }
    return INVALID;
}
void addToCollection(WordDef * wordDef, int wordType){
    codeCollection[ic].word = wordDef;
    codeCollection[ic].wordType = wordType;
    ic++;
}

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era){
    WordDef* wordDef = (WordDef*)malloc(sizeof(WordDef));

    if(wordDef != NULL){
        wordDef -> command.notUsed = NOT_USED_DEFAULT_VALUE;
        wordDef -> command.opCode = opCode;
        wordDef -> command.group = group;
        wordDef -> command.dest = 0;
        wordDef -> command.src = 0;
        wordDef -> command.ERA = Era;
        return wordDef;
    }
    printErr("can not create a command Word\n");
    return NULL;
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
    int number = -1, number2 = -1;
    char operAttr[LINE_LENGTH]  = "", extraData[LINE_LENGTH] = "";
    
    if(sscanf(oper," %*[#]%*[-+]%d %[^\n]",&number,extraData) >= 1 ||
        sscanf(oper," %*[#]%d %[^\n]",&number,extraData) >= 1){
            if(strlen(extraData) == 0)
                return instant;
            return invalidOperand;
    } else if(sscanf(oper," %*[r]%d %[^\n]",&number,extraData) >= 1){
        if(number >= 0 && number <= 7 && strlen(extraData) == 0)
            return directRegister;
        return invalidOperand;
    } else if(sscanf(oper," %*[^[][%d-%d] %[^\n]",&number,&number2,extraData) >= 2 ||
            sscanf(oper," %*[^[][%d-] %[^\n]",&number,extraData) >= 1 ||
            sscanf(oper," %*[^[][-%d] %[^\n]",&number,extraData) >= 1){
        if(strlen(extraData) == 0 && number < number2 && number >= 0 && number2 <= 13)
            return dynamic;
        return invalidOperand;
    } else if(sscanf(oper," %[^ \t\r] %[^\n]",operAttr,extraData) >= 1 && strchr(oper,'[') == NULL
             && strchr(oper,']') == NULL){
        if(strlen(extraData) == 0)
            return direct;
        return invalidOperand;
    } else {
        return invalidOperand;        
    }
}

int isValidBlockAddressTypeForAction(int sourcingType, int* validBLA){
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
    int numberOfRows = 1, currentIc = ic;
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
    int i = 0;
    
    printf("|notUsed|Group|opCode|SRC|DEST|ERA|\n");
    for(i = 0; i < ic; i++)
    {
        word = &codeCollection[i];
        if(word != NULL && word -> word != NULL){
            if(word -> wordType == commandType){
                printf("%d|%d|%d|%d|%d|%d\n",word -> word -> command.notUsed,word -> word -> command.group,word -> word -> command.opCode,
                word -> word -> command.src,word -> word -> command.dest,word -> word -> command.ERA);
            }
        }
    }
    
}