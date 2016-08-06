#include "assembler.h" 

int main(int argc, char* argv[])
{
    /*int counter=0;*/
 	FILE * inputP;
    int icPointer = -1;
    int currentFileIndex = 1;
    char fileName[MAX_FILE_NAME], suffix[3];

    if(argc <= 1){
        printErr(" no files specified for the assembler\n");
        return 0;
    } 

    while(currentFileIndex < argc){
        
        if(sscanf(argv[currentFileIndex],"%[^.].%s",fileName,suffix) == 2)
        {
            if((inputP = fopen(argv[currentFileIndex],"r")) == NULL){
                printErr("Invalid input file %s\n",fileName);
            } else {

                /* creeate output files */
                createFile(fileName);
                /* perfoem the first iteration */
                firstIteration(inputP);
                /* get the ic pointer value after the first iteration */
                icPointer = getICPointer();
                /* update refrences according to the ic counter value */
                updateSymbolTableRefrences(icPointer);
                /* rewind to the begining of the file */
                rewind(inputP);
                /* perfoem the second iteration */
                secondIteration(inputP);
                /* print the output to the .ob file */
                printCodeCollection();
                printDataCollection(icPointer);

                /* clean all collections used by the assembler */
                cleanAllCollections();
                
                /* close all files */
                closeFiles();
                if(inputP != NULL)
                    fclose(inputP);
            }
        } else {
            printErr("invalid file name for assembler%s,%s\n",fileName,suffix);
            
        }

        currentFileIndex++;
    }
    
    return 0;
}

void cleanAllCollections(){
    disposeSymbolsTable();
    clearData();
    cleanCodeCollection();
}