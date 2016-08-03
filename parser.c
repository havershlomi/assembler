#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"
#include "symbolsTable.h"
#include "fileHandler.h"


char *illegalLabelNames[24]={
    "r0","r1","r2","r3","r4","r5","r6","r7",
    "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"
};

const long ILLIEGAL_LABELS_LENGTH = (sizeof(illegalLabelNames)/(sizeof(char*)));

int getInstructionType(const char* instruction){
    if(strcmp(instruction,"data") == 0)
        return data;
    else if(strcmp(instruction,"string") == 0)
        return string;
    else if(strcmp(instruction,"extern") == 0)
        return external;
    else if(strcmp(instruction,"entry") == 0)
        return entry;
    else
        return invalidInstruction;
}

void firstMove(FILE* input){
    int instructionType = -1, dcPointer = -1, isExternal = false, commandType = none, icPointer = -1;
    int hasLabel = false;
    char label[30] = "", dataType[LINE_LENGTH] = "", action[4] = "",actionAttr[LINE_LENGTH] = "",*labelPtr;
    char labelDelimeter;
	char str[LINE_LENGTH] = "", dataStr[LINE_LENGTH] = "";
    Symbol *symbol;
        
    while(fgets(str,LINE_LENGTH,input) != NULL){
        printf("new sentence:  %s\n",str);
        hasLabel = false;
        actionAttr[0] = '\0';
        
        if(str[0] != ';')
        {
            /*try to find label*/
            if(sscanf(str,"%[^ \r\t:]%[:]",label,&labelDelimeter) == 2){
                if(!isspace(str[0])  && isValidLabel(label) == true){
                    hasLabel = true; 
                    labelPtr = label;
                    printf("symbolFound:%s\n",label);
                } else {
                    printErr("label \"");
                    printErr(label);
                    printErr("\" is invalid.\n");
                }
            }
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %*[.]%s %[^\n]",dataType,dataStr) == 2)
                || (hasLabel == false && sscanf(str," %*[.]%s %[^\n]",dataType,dataStr) == 2)){
                
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == string){
                    dcPointer =  addStringInstructionToDC(dataStr);
                    isExternal = false;
                    commandType = instructionCommand;
                }
                else if(instructionType == data){
                    dcPointer = addDataInstructionToDC(dataStr);
                    isExternal = false;
                    commandType = instructionCommand;
                } 
                else if(instructionType == external){
                    if(isValidLabel(dataStr) == true){
                        isExternal = true;
                        dcPointer = 0;
                        commandType = none;
                        labelPtr = dataStr;
                    } else {
                        printErr("illegal symbol for .extern instruction \n");
                    }
                } 
                if((hasLabel == true && (instructionType == data || instructionType == string))
                    || (instructionType == external && isExternal == true)){
                    symbol = createSymbol(labelPtr, dcPointer, commandType, isExternal);
                    if(tryAddSymbol(symbol) == true){
                        printf( "added : %s\n", symbol -> name);
                    }
                }
            } else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t\n:] %[^\n] ",action, actionAttr) >= 1)
                || (hasLabel == false && sscanf(str," %[^ \r\t\n:] %[^\n] ",action,actionAttr) >= 1)){
                /*handle action*/
                icPointer  = getActionBlockAddressType(action,actionAttr);
                printf("%s|%d,%s\n",label,icPointer,actionAttr);
                
                if(hasLabel == true && icPointer != INVALID){
                    symbol = createSymbol(label, icPointer, actionCommand, isExternal);

                    if(tryAddSymbol(symbol) == true){
                        printf( "added : %s\n", symbol -> name);
                    }
                }
            } else if(sscanf(str," %[^ \t\r\n] ",dataStr) == 1){
                /*if not empty line */
                printErr("Invalid command \n");
            }
        }
	}
}


void secondMove(FILE* input, const char* fileName){
    int instructionType = -1,  refrencePointer = -1;
    int hasLabel = false;
    char label[30] = "", dataType[LINE_LENGTH] = "", action[4] = "",actionAttr[LINE_LENGTH] = "";
    char labelDelimeter;
	char str[LINE_LENGTH] = "", dataStr[LINE_LENGTH] = "";
    char entryFileName[MAX_FILE_NAME] = ""; 
    
    sprintf(entryFileName, "%s.%s",fileName,"ent");

    resetIc();
    while(fgets(str,LINE_LENGTH,input) != NULL){
        printf("new sentence:  %s\n",str);
        hasLabel = false;
        actionAttr[0] = '\0';

        if(str[0] != ';')
        {
            
            /*try to find label*/
            if(sscanf(str,"%[^ \r\t:]%[:]",label,&labelDelimeter) == 2){
                if(!isspace(str[0])  && isValidLabel(label) == true){
                    hasLabel = true; 
                    
                } else {
                    printErr("label '%s' is invalid\n",label);
                }
            }
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %*[.]%s %[^\n]",dataType,dataStr) == 2)
                || (hasLabel == false && sscanf(str," %*[.]%s %[^\n]",dataType,dataStr) == 2)){
                
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == entry){
                    if(isValidLabel(dataStr) == true){
                        refrencePointer = getSymbolRefrenceByName(dataStr);
                        if(refrencePointer == SYMBOL_NOT_FOUND)
                        {
                            printErr("Symbol: %s was not found in the symbol table\n",dataStr);
                        } else {
                            fileWrite(entryFileName,"%s %d\n",dataStr,refrencePointer);
                        }
                    }
                    else
                    {
                        printErr("illegal symbol for .entry instruction \n");
                    
                    }
                } 

            } else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t\n] %[^\n] ",action, actionAttr) >= 1)
                || (hasLabel == false && sscanf(str," %[^ \r\t\n:] %[^\n] ",action,actionAttr) >= 1)){
                
                /*handle action*/
                
                addActionToCodeCollection(action,actionAttr);
            
            } else if(sscanf(str," %[^ \t\r\n] ",dataStr) == 1){
                /*if not empty line */
                printErr("Invalid command \n");
            }
        }
	}
}

int isValidLabel(char *label){
    char validLabel[LINE_LENGTH] = "" ,extraData[LINE_LENGTH] = "";
    int i = 0;
    if(sscanf(label," %[^ \t\r] %s ",validLabel,extraData) == 1 &&
        !isdigit(label[0]) && isalpha(label[0]) && strlen(label) <= LABAL_MAX_LENGTH){
        for(i = 0; i < 24; i++){
            if(strcmp(label,illegalLabelNames[i]) == 0){
                return false;
            }
        }
        return true;
    }
    return false;
}



int getActionBlockAddressType(char *action,char *actionAttr){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int blockAddressSourceType = -1, blockAddressDestType = -1;

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
                        return getActionRefrenceinMemory(blockAddressSourceType, blockAddressDestType); 
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
                return getActionRefrenceinMemory(notUsedOper, blockAddressDestType);
            }   
        }
        else {
            /*handle actions that have zero operands*/
            printf("%s",actionAttr);
            if(actionAttr != NULL && sscanf(actionAttr," %[^\n] ",extraData) == 1){
                printErr(action);
                printErr(" can not accept operands\n");               
            } else
                return getActionRefrenceinMemory(notUsedOper, notUsedOper);
        }
    }
    else {
        printErr("the action name is invalid\n");
    }
    return INVALID;
}

/*gets the  string instruction and extract all the numbers and save them*/
int addStringInstructionToDC(char *dataStr){
    char stringEnd[LINE_LENGTH], instructionData[LINE_LENGTH] = "";
    
    /*get the string only if valid and put it in the data collection*/
    if(sscanf(dataStr,"\"%[^\"] %[\"]",instructionData, stringEnd) == 2)
    {
        return addStringData(instructionData);  
    } else {
        printf("invalid string\n");
        return INVALID;
    }
}

/*gets the data instruction and extract all the numbers and save them*/
int addDataInstructionToDC(char *dataStr){
    char *numberPart,numberDelimiter[1] = ",";
    int numbers[LINE_LENGTH] , numberFactor = 1;
    int j = 0, i = 0, dataPosition = -1;
    int  startReadingNumber = false, finishReadingNumber = false, invalidData = false;
    numberPart = strtok(dataStr,numberDelimiter);
    j = 0;
    while(numberPart != NULL && invalidData == false){
        startReadingNumber = false;
        finishReadingNumber = false;
        numbers[j] = 0;
        numberFactor = 1;
        /*iterate over number string*/
        for(i = 0;i < strlen(numberPart); i++){
            if(isdigit(numberPart[i]) && finishReadingNumber == false){
                startReadingNumber = true;                                
                numbers[j] = numbers[j] * 10 + (numberPart[i]-'0');
            } else if(startReadingNumber == true && isspace(numberPart[i])) {
                finishReadingNumber = true;
            } else if(finishReadingNumber == true){
                invalidData = true;
                break;
            }
            else if(numberPart[i] == '+' || numberPart[i] == '-') {
                startReadingNumber = true;
                if(numberPart[i] == '-')
                    numberFactor = -1; 
            }
        }
        if(startReadingNumber == false){
            invalidData = true;
        } else {
            numbers[j] *= numberFactor;
            j++;     
        }
        numberPart = strtok(NULL,numberDelimiter);
    }
    if(invalidData == true){
        printErr("invalid data\n");
        invalidData = false;
    } else {                        
        dataPosition = addNumbersData(numbers,j);
    }
    return dataPosition;
}