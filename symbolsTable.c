#include "dataStructure.h"
#include "symbolsTable.h"
#include "utils.h"

int tryAddSymbol(Symbol* symbol)
{
    SymbolNode *newNode = 0, *prevPtr, *symbolPtr;
    char* symbolName = (*symbol).name;
    if(symbolTable != NULL)
    {
        /*check if symbol is not in the table already*/
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            if(symbolPtr -> value.name != '\0' &&
                strcmp(symbolPtr -> value.name, symbolName) == 0)
            {
                printErr("symbol: ");
                printErr(symbolName);
                printErr(" is already defined\n");
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
        /*adsd the new symbol to the table*/
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
        printErr("can not locate space to the new symbol\n");
        return false;
    }
    
    return true;
}

int getSymbolRefrenceByName(char* symbolName){
    SymbolNode *symbolPtr;
    
    if(symbolTable != NULL)
    {
        /*find the symbol in the list*/
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            if(symbolPtr -> value.name != '\0' &&
                strcmp(symbolPtr -> value.name, symbolName) == 0)
            {
                return symbolPtr -> value.refrence;               
            }
            symbolPtr = symbolPtr -> next;
        }
    }
    return SYMBOL_NOT_FOUND;
}

void disposeSymbolsTable(){
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