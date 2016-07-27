#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../utils.h"
#include "../parser.h"
#include "../codeCollection.h"


 
int main(int argc, char* argv[])
{
    int lineCounter = 0;
    int instructionType=-1;
    /*int counter=0;*/
    int hasLabel = false;
    char label[30], dataType[10],action[4];
    char labelDelimeter;
	char str[80];
    Action *selectedAction;
	FILE * inputP,*outputP = NULL;

    /*load the files if needed*/
    if(argc > 1){
        if((inputP = fopen(argv[1],"r")) == NULL){
            fprintf(stderr,"Invalid input file\n");
            return 0;
        }
        /*replace the standared input with the file providerd by the user*/
        stdin = inputP;
    }
    if(argc == 3){
        if((outputP = fopen(argv[2],"w")) == NULL){
            fprintf(stderr,"Invalid output file\n");
            return 0;
        }
        /*replace the standared output with the file providerd by the user*/
        stdout = outputP;
    }
    getActionByName("stop");
    while(fgets(str,80,stdin) != NULL){
        printf("new sentence:  %s\n",str);
        hasLabel = false;
        lineCounter++;
        if(str[0] == ';')
            printf("ignore me\n");
        else
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
                || sscanf(str,"%*[ \t\r]%*[.]%s",dataType) == 1){
                printf("found token: %s\n",dataType);
                instructionType = getInstructionType(dataType);
                if(instructionType == invalidInstruction){
                    printErr("invalid instruction was found\n");
                }
                else {
                    /*get the numbers or string and add it to the data collection*/
                }
            } else if((hasLabel == true && sscanf(str,"%*s %s",action) == 1)
                || sscanf(str," %s",action) == 1){
                /*handle action*/
                printf("action: %s\n",action);
                selectedAction = getActionByName(action);

                printf("Action: %s \n",selectedAction -> name);
            }

           /* for(i = 0; i < strlen(str);i++){
                if(isspace(str[i]))
                {
                    break; 
                } else if(str[i] == labelToken){
                    hasLabel = true;
                    printf("found token: %d\n",hasLabel);
                }else
                    printf("%c\n",str[i]);
            }*/
        }
	}
    printf("\nlineCounter: %d \n",lineCounter);

	if(inputP != NULL)
		fclose(inputP);
	if(outputP != NULL)
		fclose(outputP);

	
	return 0;
}

