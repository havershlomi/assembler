#include "dataStructure.h"
#include "symbolsTable.h"
#include "utils.h"

Symbol* createSymbol(char *label, int refrence, int commandType, int isExternal)
{
    char  *trimmedLabel;
    Symbol* symbol;
    symbol = (Symbol*)malloc(sizeof(Symbol));

    trimmedLabel = trimWhiteSpace(label);

    if(symbol)
    {
        memcpy(symbol -> name, trimmedLabel, LABAL_MAX_LENGTH);
        symbol -> isExternal = isExternal;
        symbol -> icRefrence = refrence;
        symbol -> commandType = commandType;
        symbol -> dcRefrence = refrence;
        return symbol;
    } 
    else 
    {
        printf("can not allocate space to the new symbol\n");
        return NULL;
    }
}

int tryAddSymbol(Symbol* symbol)
{
    SymbolNode *newNode = 0, *prevPtr, *symbolPtr;
    char* symbolName = (*symbol).name;
    if(symbol == NULL)
    {
	    return false;        
    }
    if(symbolTable != NULL)
    {
        /*check if symbol is not in the table already*/
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            if(symbolPtr -> value.name != '\0' &&
                strcmp(symbolPtr -> value.name, symbolName) == 0)
            {
                printErr("symbol: '%s' is already defined\n", symbolName);
                return false;               
            }
    
            prevPtr = symbolPtr; 
            symbolPtr = symbolPtr -> next;
        }
        symbolPtr = prevPtr;
    }
    
    newNode = (SymbolNode*)malloc(sizeof(SymbolNode));

    if(newNode)
    {
        /*add the new symbol to the table*/
        newNode -> value = *symbol;
        newNode -> next = NULL;
        if(symbolTable == NULL)
        {
            symbolTable = newNode;
        }
        else
        {
            symbolPtr -> next = newNode;
        }
    } 
    else
    {
        printf("can not allocate space to the new symbol\n");
        return false;
    }
    
    return true;
}

Symbol* getSymbolByName(char* symbolName){
    SymbolNode *symbolPtr;
    
    symbolName = trimWhiteSpace(symbolName);
    if(symbolTable != NULL)
    {
        /*find the symbol in the list*/
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            if(symbolPtr -> value.name != '\0' &&
                strcmp(symbolPtr -> value.name, symbolName) == 0)
            {
                return &(symbolPtr -> value);               
            }
            symbolPtr = symbolPtr -> next;
        }
    }
    return NULL;
}

int getSymbolRefrenceByName(char* symbolName)
{
    Symbol *symbolPtr = getSymbolByName(symbolName);
    if(symbolPtr != NULL)
        return symbolPtr -> icRefrence;

    return SYMBOL_NOT_FOUND;
}

void disposeSymbolsTable()
{
    SymbolNode *symbolPtr, *currentPtr;
    
    if(symbolTable != NULL)
    {
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            currentPtr = symbolPtr;
            symbolPtr =  symbolPtr -> next;
            free(currentPtr);
        }
        symbolTable = NULL;
    }
}

void updateSymbolTableRefrences(int icPointer)
{
     SymbolNode *symbolNodePtr;
     Symbol currentSymbol;
    
    if(symbolTable != NULL)
    {
        /*check if symbol is not in the table already*/
        symbolNodePtr = symbolTable;
        while(symbolNodePtr != NULL)
        {
            currentSymbol = (symbolNodePtr -> value);
            
            if(currentSymbol.isExternal == false && currentSymbol.commandType == instructionCommand){
                symbolNodePtr -> value.icRefrence = currentSymbol.icRefrence + icPointer;
                
            }
            symbolNodePtr = symbolNodePtr -> next;
        }
    }
}
