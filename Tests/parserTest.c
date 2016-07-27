#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../utils.h"

 
int main(int argc, char* argv[])
{
    int lineCounter = 0;
    /*int counter=0;*/
    int hasLabel = false;
    char label[30];
    char labelDelimeter;
	char str[80];
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
    
    while(fgets(str,80,stdin) != NULL){
        printf("new sentence: %s",str);
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
            if(hasLabel == true){}
 
            /*check if instruction
            if(sscanf(str,"%[^\t\n\r\v\f.s]: ",label) == 1){
                if(!isspace(label[0]) && !isdigit(label[0]) && isalpha(label[0])){
                    printf("found token: %s\n",label);
                    hasLabel = true;
                }
            }*/

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

