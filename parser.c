#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#include "binaryConvertor.h"

/* a list with reserved names */
char *illegalLabelNames[24]={
    "r0","r1","r2","r3","r4","r5","r6","r7",
    "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"
};
/* the length of the list */
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

void firstIteration(FILE* input){
    int instructionType = -1, dcPointer = -1, isExternal = false, commandType = none, icPointer = -1;
    int hasLabel = false;
    char label[LABAL_MAX_LENGTH] = "", dataType[LINE_LENGTH] = "", action[ACTION_MAX_LENGTH] = ""
    ,actionAttr[LINE_LENGTH] = "",*labelPtr;
    char labelDelimeter;
	char str[LINE_LENGTH] = "", dataStr[LINE_LENGTH] = "";
    Symbol *symbol;
    
    /* read each row */
    while(fgets(str,LINE_LENGTH,input) != NULL){
        hasLabel = false;
        actionAttr[0] = '\0';
        
        if(str[0] != ';')
        {
            /*try to find label*/
            if(sscanf(str,"%[^ \r\t:]%[:]",label,&labelDelimeter) == 2){
                if(!isspace(str[0])  && isValidLabel(label) == true){
                    hasLabel = true; 
                    labelPtr = label;
                } else {
                    printErr("label \"%s\" is invalid.\n",label);
                }
            }
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %*[.]%s %[^\n]",dataType,dataStr) == 2)
                || (hasLabel == false && sscanf(str," %*[.]%s %[^\n]",dataType,dataStr) == 2)){
                /* find the instruction */
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
                    /* add label to the symbol table */
                    symbol = createSymbol(labelPtr, dcPointer, commandType, isExternal);
                    if(symbol == NULL || tryAddSymbol(symbol) == false){
                        printf("there is a problem adding %s to the symbol table\n",symbol -> name);
                    }
                }
            } else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t\n:] %[^\n] ",action, actionAttr) >= 1)
                || (hasLabel == false && sscanf(str," %[^ \r\t\n:] %[^\n] ",action,actionAttr) >= 1)){
                /*handle action*/
                icPointer  = calculateActionIcPointer(action,actionAttr);
                
                if(hasLabel == true && icPointer != INVALID){
                    /*  add the action to the symbol table */
                    symbol = createSymbol(label, icPointer, actionCommand, isExternal);
                    if(symbol == NULL || tryAddSymbol(symbol) == false){
                        printf("there is a problem adding %s to the symbol table\n", symbol -> name);
                    }                
                }
            } else if(sscanf(str," %[^ \t\r\n] ",dataStr) == 1){
                /*if not empty line */
                printErr("Invalid command \n");
            }
        }
	}
}


void secondIteration(FILE* input){
    int instructionType = -1,  refrencePointer = -1;
    int hasLabel = false;
    char label[LABAL_MAX_LENGTH] = "", dataType[LINE_LENGTH] = "",
     action[ACTION_MAX_LENGTH] = "",actionAttr[LINE_LENGTH] = "";
    char labelDelimeter;
	char str[LINE_LENGTH] = "", dataStr[LINE_LENGTH] = "",*lineNumber;
    /* start counting commands again */
    resetIc();
    /* loop through each line again */
    while(fgets(str,LINE_LENGTH,input) != NULL){
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
                
                /* find instruction type */
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == entry){
                    /* if entry label is valid write it to entry file */                    
                    if(isValidLabel(dataStr) == true){
                        refrencePointer = getSymbolRefrenceByName(dataStr);
                        if(refrencePointer == SYMBOL_NOT_FOUND)
                        {
                            printErr("Symbol: %s was not found in the symbol table\n",dataStr);
                        } else {
                            lineNumber = getSpecialBase8String(refrencePointer);
                            entryWriteToFile("%s %s\n",dataStr,lineNumber);
                            free(lineNumber);
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
    /* try to get label check if valid and not part of the reserved words list */
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

/*gets the  string instruction and extract all the numbers and save them*/
int addStringInstructionToDC(char *dataStr){
    char stringEnd[LINE_LENGTH], instructionData[LINE_LENGTH] = "";
    
    /* get the string only if valid and put it in the data collection*/
    if(sscanf(dataStr,"\"%[^\"] %[\"]",instructionData, stringEnd) == 2)
    {
        return addStringData(instructionData);  
    } else {
        printErr("invalid string was specified\n");
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
    /* loop over each part of the .data string */
    while(numberPart != NULL && invalidData == false){
        /* reset this values */
        startReadingNumber = false;
        finishReadingNumber = false;
        numbers[j] = 0;
        numberFactor = 1;

        /*iterate over number string*/
        for(i = 0;i < strlen(numberPart); i++)
        {
            /* check if valid digit */
            if(isdigit(numberPart[i]) && finishReadingNumber == false)
            {
                startReadingNumber = true;                                
                numbers[j] = numbers[j] * 10 + (numberPart[i]-'0');
            }
            else if(startReadingNumber == true && isspace(numberPart[i])) 
            {
                finishReadingNumber = true;
            } 
            else if(finishReadingNumber == true)
            {
                invalidData = true;
                break;
            }
            else if(numberPart[i] == '+' || numberPart[i] == '-') 
            {
                /* check if the number has special sign (+/-) */
                startReadingNumber = true;
                if(numberPart[i] == '-')
                    numberFactor = -1; 
            }
        }

        if(startReadingNumber == false)
        {
            invalidData = true;
        } 
        else
        {
            numbers[j] *= numberFactor;
            j++;     
        }
        /* get the next part of the number */
        numberPart = strtok(NULL,numberDelimiter);
    }
    
    if(invalidData == true)
    {
        printErr("invalid data\n");
        invalidData = false;
    }
    else
    {                        
        dataPosition = addNumbersData(numbers,j);
    }
    /* return the first position we enetered the .data numbers */
    return dataPosition;
}