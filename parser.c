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
    int instructionType=-1;
    int hasLabel = false;
    char label[30], dataType[80],action[4];
    char labelDelimeter;
	char str[80],data;
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
            if((hasLabel == true && sscanf(str,"%*s %*[.]%s",dataType) == 1)
                || sscanf(str," %*[.]%s",dataType) == 1){
                printf("found token: %s\n",dataType);
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else if(instructionType == string){
                    if(sscanf(str,"%*s \"%s%*[\"]",&data) == 1)
                    {
                        printf("this is my data:'%s'\n",&data);
                        printf("%d",addData(&data));  
                    }else{
                        printf("couldn't find\n");
                    }
                    /*get the numbers or string and add it to the data collection*/
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