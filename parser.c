#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"
#include "symbolsTable.h"


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
        return externl;
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
                else if(instructionType == externl){
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
                    || (instructionType == externl && isExternal == true)){
                    symbol = (Symbol*)malloc(sizeof(Symbol));
                    memcpy(symbol -> name,labelPtr,30);/*copy string into name and extrac into to a function*/
                    symbol -> isExternal = isExternal;
                    symbol -> refrence = dcPointer;
                    symbol -> commandType = commandType;

                    if(tryAddSymbol(symbol) == true){
                        printf( "added : %s\n", symbol -> name);
                    }
                }
            } else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t:] %[^\n] ",action, actionAttr) == 2)
                || (hasLabel == false && sscanf(str," %s %[^\n] ",action,actionAttr) == 2)){
                /*handle action*/
                icPointer  = getActionBLAddress(action,actionAttr,hasLabel);
                    printf("%s\n",label);
                
                if(hasLabel == true && icPointer != INVALID){
                    symbol = (Symbol*)malloc(sizeof(Symbol));
                    memcpy(symbol -> name,label,30);
                    symbol -> isExternal = false;
                    symbol -> refrence = icPointer;
                    symbol -> commandType = actionCommand;

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

int getActionBLAddress(char *action,char *actionAttr, const int hasLabel){
    Action *selectedAction;
    char firstOper[LINE_LENGTH] = "", secondOper[LINE_LENGTH] = "", extraData[LINE_LENGTH] = "";
    int blockAddressSourceType = -1, blockAddressDestType = -1;

    selectedAction = getActionByName(action);
    printf("%s| %s\n",action,actionAttr);
    if(selectedAction != NULL){
        printf("Action: %s \n",selectedAction -> name);
        if(selectedAction -> numOfOperands == 2){
            if(sscanf(actionAttr," %[^ \r\t:,] , %[^ \r\t:] %[^\n] ",firstOper,secondOper,extraData) >= 2){
                printf("%s \n %s \n %s\n",firstOper,secondOper,extraData);
                if(strlen(extraData) != 0){
                    printErr(action);
                    printErr(" can not get more than 2 operands\n");
                } else {
                    blockAddressSourceType = getOperandType(firstOper);
                    blockAddressDestType = getOperandType(secondOper);

                    if(isValidBlockAddressTypeForAction(blockAddressSourceType,selectedAction -> sourceOper) == true
                        && isValidBlockAddressTypeForAction(blockAddressDestType,selectedAction -> destOper) == true){
                        return getActionRefrenceinMemory(blockAddressSourceType, blockAddressDestType); 
                    } else {
                        printErr("invalid block address method for action '");
                        printErr(action);
                        printErr("'\n");                
                    }    
                }
            } else {
                printErr(action);
                printErr(" except to get 2 operands\n");                
            }
        } else if(selectedAction -> numOfOperands == 1){
            blockAddressDestType = getOperandType(actionAttr);
            if(isValidBlockAddressTypeForAction(blockAddressDestType,selectedAction -> destOper) == false){
                printErr("invalid block address method for action '");
                printErr(action);
                printErr("'\n");
            } else {
                return getActionRefrenceinMemory(notUsedOper, blockAddressDestType);
            }   
        }
        else {
            /*handle actions that have zero operands*/
            if(sscanf(actionAttr," %[^\n] ",extraData) == 1){
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
    char stringEnd[1], instructionData[LINE_LENGTH] = "";
    /*get the string only if valid and put it in the data collection*/
    if(sscanf(dataStr,"\"%[^\"] %[\"]",instructionData,stringEnd) == 2)
    {
        return addStringData(instructionData);  
    }else{
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