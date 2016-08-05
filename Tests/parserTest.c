#include <stdio.h>
#include <string.h>

#include "../utils.h"
#include "../parser.h"
#include "../codeCollection.h"
#include "../symbolsTable.h"
#include "../fileHandler.h"


 
int main(int argc, char* argv[])
{
    /*int counter=0;*/
 	FILE * inputP,*outputP = NULL;
     int icPointer = -1;

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
    createFile("outputFile");
    firstMove(inputP);
    icPointer = getICPointer();
    printTable();
    updateSymbolTableRefrences(icPointer);
    printf("\nUpdated Table\n\n");
    printTable();

    rewind(inputP);
    secondMove(inputP);
    printCodeCollection();

    closeFiles();
	if(inputP != NULL)
		fclose(inputP);
	if(outputP != NULL)
		fclose(outputP);

	
	return 0;
}

