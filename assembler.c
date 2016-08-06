#include "assembler.h" 

int main(int argc, char* argv[])
{
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
            if((inputP = fopen(argv[currentFileIndex],"r")) == NULL)
            {
                printErr("can't read file '%s'\n",argv[currentFileIndex]);
            } 
            else 
            {
                resetErrorFlag();
                /* creeate output files */
                createFile(fileName);

                /* perfoem the first iteration */
                firstIteration(inputP);

                /* get the ic pointer value after the first iteration */
                icPointer = getICPointer();

                printObjFileHeader();

                /* update refrences according to the ic counter value */
                updateSymbolTableRefrences(icPointer);

                /* rewind to the begining of the file */
                rewind(inputP);

                /* perfoem the second iteration */
                secondIteration(inputP);

                if(hasErrorInInput() == false)
                {
                    /* print the output to the .ob file */
                    printCodeCollection();
                    printDataCollection(icPointer);
                } 
                else 
                {
                    deleteFiles();
                }
                    
                /* clean all collections used by the assembler */
                cleanAllCollections();
                
                /* close all files */
                closeFiles();

                if(inputP != NULL)
                    fclose(inputP);
            }
        } 
        else 
        {
            printf("invalid file name for assembler '%s'\n",fileName);
        }
        currentFileIndex++;
    }
    return 0;
}

void cleanAllCollections()
{
    disposeSymbolsTable();
    clearData();
    cleanCodeCollection();
    resetIc();
}

void printObjFileHeader()
{
    char *icNumber,*dcNumber;
    int icCollectionSize, dcCollectionSize;

    icCollectionSize = getICCollectionSize();
    dcCollectionSize = getDCPointer();

    objWriteToFile("Base 8 Special Address  |   Base 8 special code\n");

    icNumber = getSpecialBase8String(icCollectionSize);
    dcNumber = getSpecialBase8String(dcCollectionSize);

    objWriteToFile("%s  %s\n", icNumber, dcNumber);

    free(icNumber);
    free(dcNumber);
}