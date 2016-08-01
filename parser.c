#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"


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

void parse(FILE* input){
    int instructionType = -1, dataPosition = -1;
    int hasLabel = false;
    char label[30], dataType[80],action[4];
    char labelDelimeter;
	char str[80], instructionData[80], dataStr[80],stringEnd[1];

    Action *selectedAction;

     while(fgets(str,80,input) != NULL){
        printf("new sentence:  %s\n",str);
        hasLabel = false;
        
        if(str[0] != ';')
        {
            /*try to find label*/
            if(sscanf(str,"%[^:]%[:]",label,&labelDelimeter) == 2){
                if(!isspace(str[0]) && !isdigit(label[0]) && isalpha(label[0]) && strlen(label) <= LABAL_MAX_LENGTH){
                    hasLabel = true;
                
                } else {
                    printErr("label \"");
                    printErr(label);
                    printErr("\" is invalid.\n");
                }
            }
 
            /*check if instruction*/
            if((hasLabel == true && sscanf(str,"%*s %*[.]%s %[^\n]",dataType,dataStr) == 2)
                || sscanf(str," %*[.]%s %[^\n]",dataType,dataStr) == 2){
                printf("found token: %s data:'%s'\n",dataType,dataStr);
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == string){
                    /*get the string only if valid and put it in the data collection*/
                    if(sscanf(dataStr,"\"%[^\"] %[\"]",instructionData,stringEnd) == 2)
                    {
                        /*printf("this is my data:'%s'\n",instructionData);*/
                        dataPosition  = addStringData(instructionData);
                        if(hasLabel == true){
                            /*add to symbol label*/
                            printf("%d",dataPosition);
                        }  
                    }else{
                        printf("invalid string\n");
                    }
                    /*get the numbers or string and add it to the data collection*/
                }
                else if(instructionType == data){
                    parseDataInstruction(dataStr,hasLabel);
                }
                 
            } else if((hasLabel == true && sscanf(str,"%*s %s",action) == 1)
                || sscanf(str," %s",action) == 1){
                /*handle action*/
                
                selectedAction = getActionByName(action);
                if(selectedAction == NULL){
                    printErr("the action name is invalid\n");
                    /*check action
                    cut the rest of the string and then pass it to the action handler */

                }
                else
                    printf("Action: %s \n",selectedAction -> name);
            }
        }
	}
   
}

void parseDataInstruction(char *dataStr, const int hasLabel){
    char *numberPart,numberDelimiter[1] = ",";
    int numbers[80] , numberFactor = 1;
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
        if(hasLabel == true){
            /*add to symbol label*/
            printf("%d",dataPosition);
        }
    }
}