#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#include "binaryConvertor.h"

#define ILLIEGAL_LABELS_LENGTH 24

/* a list with reserved names */
char *illegalLabelNames[ILLIEGAL_LABELS_LENGTH]={
    "r0","r1","r2","r3","r4","r5","r6","r7",
    "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"
};


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

void firstIteration(FILE* input)
{
    int instructionType = -1, dcPointer = -1, isExternal = false, commandType = none, icPointer = -1;
    int hasLabel = false;
    /* char arrays for sscanf operations */
    char label[LABAL_MAX_LENGTH] = "", dataType[LINE_LENGTH] = "", action[ACTION_MAX_LENGTH] = ""
    ,actionAttr[LINE_LENGTH] = "",*labelPtr;
    char labelDelimeter;
	char str[LINE_LENGTH] = "", dataStr[LINE_LENGTH] = "";
    Symbol *symbol;
    
    /* read each row */
    while(fgets(str,LINE_LENGTH,input) != NULL)
    {
        hasLabel = false;
        actionAttr[0] = '\0';
        
        if(str[0] != ';')
        {
            /*try to find label*/
            if(sscanf(str," %[^:]%[:]",label,&labelDelimeter) == 2)
            {
                if(!isspace(str[0]) && isValidLabel(label) == true)
                {
                    hasLabel = true; 
                    labelPtr = label;
                } 
                else 
                {
                    printErr("'%s' is an invalid lanbel name.\n",label);
                    continue;
                }
            }
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %*[.]%s %[^\n] ",dataType,dataStr) == 2)
                || (hasLabel == false && sscanf(str," %*[.]%s %[^\n] ",dataType,dataStr) == 2))
            {
                /* find the instruction */
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction)
                {
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == string)
                {
                    dcPointer =  addStringInstructionToDC(dataStr);
                    if(dcPointer == INVALID)
                        printErr("at row: %s",str);
                    isExternal = false;
                    commandType = instructionCommand;
                }
                else if(instructionType == data)
                {
                    dcPointer = addDataInstructionToDC(dataStr);
                    if(dcPointer == INVALID)
                        printErr("at row: %s",str);
                    isExternal = false;
                    commandType = instructionCommand;
                } 
                else if(instructionType == external)
                {
                    labelPtr = trimWhiteSpace(dataStr);
                    if(isValidLabel(dataStr) == true)
                    {
                        isExternal = true;
                        dcPointer = 0;
                        commandType = none;
                    } 
                    else 
                    {
                        printErr("illegal symbol for .extern instruction \n");
                    }
                } 
                if((hasLabel == true && (instructionType == data || instructionType == string))
                    || (instructionType == external && isExternal == true))
                {
                    /* add label to the symbol table */
                    symbol = createSymbol(labelPtr, dcPointer, commandType, isExternal);
                    tryAddSymbol(symbol);
                }
            } 
            else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t\n:] %[^\n] ",action, actionAttr) >= 1)
                || (hasLabel == false && sscanf(str," %[^ \r\t\n:] %[^\n] ",action,actionAttr) >= 1))
            {
                /*handle action*/
                icPointer  = calculateActionIcPointer(action,actionAttr);
                
                if(hasLabel == true && icPointer != INVALID)
                {
                    /*  add the action to the symbol table */
                    symbol = createSymbol(label, icPointer, actionCommand, isExternal);
                    tryAddSymbol(symbol);                                    
                }
            } 
            else if(sscanf(str," %[^ \t\r\n] ",dataStr) == 1)
            {
                /*if not empty line */
                printErr("Invalid command '%s'\n",str);
            }
        }
	}
}


void secondIteration(FILE* input){
    int instructionType = -1,  refrencePointer = -1;
    int hasLabel = false;
    /* char arrays for sscanf operations */
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
            if(sscanf(str,"%[^ \r\t:]%[:]",label,&labelDelimeter) == 2)
            {
                if(!isspace(str[0])  && isValidLabel(label) == true)
                {
                    hasLabel = true; 
                } 
                else 
                {
                    printErr("label '%s' is invalid\n",label);
                }
            }
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %*[.]%s %[^\n]",dataType,dataStr) == 2)
                || (hasLabel == false && sscanf(str," %*[.]%s %[^\n]",dataType,dataStr) == 2))
            {
                
                /* find instruction type */
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction)
                {
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == entry)
                {
                    /* if entry label is valid write it to entry file */                    
                    if(isValidLabel(dataStr) == true)
                    {
                        refrencePointer = getSymbolRefrenceByName(dataStr);
                        if(refrencePointer == SYMBOL_NOT_FOUND)
                        {
                            printErr("Symbol: '%s' was not found in the symbol table\n",dataStr);
                        } 
                        else 
                        {
                            lineNumber = getSpecialBase8String(refrencePointer);
                            entryWriteToFile("%s %s\n",dataStr,lineNumber);
                            free(lineNumber);
                        }
                    }
                    else
                    {
                        printErr("illegal symbol for .entry instruction '%s'\n",dataStr);
                    }
                } 

            } 
            else if((hasLabel == true && sscanf(str,"%*[^ \r\t:]%*[:] %[^ \r\t\n] %[^\n] ",action, actionAttr) >= 1)
                || (hasLabel == false && sscanf(str," %[^ \r\t\n:] %[^\n] ",action,actionAttr) >= 1))
            {    
                /*handle action*/
                addActionToCodeCollection(action,actionAttr);
            }
        }
	}
}

int isValidLabel(char *label)
{
    char validLabel[LINE_LENGTH] = "" ,extraData[LINE_LENGTH] = "";
    int i = 0;
    /* try to get label check if valid and not part of the reserved words list */
    if(sscanf(label," %[^ \t\r] %s ",validLabel,extraData) == 1 &&
        !isdigit(label[0]) && isalpha(label[0]) && strlen(label) <= LABAL_MAX_LENGTH)
    {
        for(i = 0; i < ILLIEGAL_LABELS_LENGTH; i++)
        {
            if(strcmp(label,illegalLabelNames[i]) == 0)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

/*gets the  string instruction and extract all the numbers and save them*/
int addStringInstructionToDC(char *dataStr)
{
    char stringEnd[LINE_LENGTH], instructionData[LINE_LENGTH] = "";
    
    /* get the string only if valid and put it in the data collection*/
    if(sscanf(dataStr,"\"%[^\"] %[\"]",instructionData, stringEnd) == 2)
    {
        return addStringData(instructionData);  
    } 
    else 
    {
        printErr("invalid string was specified ");
        return INVALID;
    }
}

/*gets the data instruction and extract all the numbers and save them*/
int addDataInstructionToDC(char *dataStr)
{
    char *numberPart,numberDelimiter[1] = ",";
    int numbers[LINE_LENGTH] , numberFactor = 1;
    int j = 0, i = 0, dataPosition = INVALID;
    int  startReadingNumber = false, finishReadingNumber = false, invalidData = false, hasSpecialSign = false;

    numberPart = strtok(dataStr,numberDelimiter);

    j = 0;
    /* loop over each part of the .data string */
    while(numberPart != NULL && invalidData == false)
    {
        /* reset this values */
        startReadingNumber = false;
        finishReadingNumber = false;
        hasSpecialSign = false;
        numbers[j] = 0;
        numberFactor = 1;

        /*iterate over number string*/
        for(i = 0;i < strlen(numberPart); i++)
        {
            /* check if valid digit and not finished to read the number yet */
            if(isdigit(numberPart[i]) && finishReadingNumber == false)
            {
                startReadingNumber = true;                                
                numbers[j] = numbers[j] * 10 + (numberPart[i]-'0');
            }
            /* check if already started reading number and found a space*/
            else if(startReadingNumber == true && isspace(numberPart[i])) 
            {
                finishReadingNumber = true;
            } 
            /* check if found a spaecial char */
            else if((numberPart[i] == '+' || numberPart[i] == '-') && hasSpecialSign == false) 
            {
                /* check if the number has special sign (+/-) */
                startReadingNumber = true;
                hasSpecialSign = true;
                if(numberPart[i] == '-')
                    numberFactor = -1; 
            }
            else if(startReadingNumber == false && isspace(numberPart[i]))
                continue;
            else if(finishReadingNumber == true || !isdigit(numberPart[i]))
            {
                invalidData = true;
                break;
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
        printErr("invalid data ");
        invalidData = false;
    }
    else
    {                        
        dataPosition = addNumbersData(numbers,j);
    }
    /* return the first position we enetered the .data numbers */
    return dataPosition;
}