#include "dataStructure.h"
#include "symbolsTable.h"
#include "utils.h"

Symbol* createSymbol(char *label, int refrence, int commandType, int isExternal){
    Symbol* symbol;
    symbol = (Symbol*)malloc(sizeof(Symbol));
    if(symbol){
        memcpy(symbol -> name,label,30);
        symbol -> isExternal = isExternal;
        symbol -> refrence = refrence;
        symbol -> commandType = commandType;
        return symbol;
    } else {
        printErr("can not allocate space to the new symbol\n");
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
                printErr("symbol: ");
                printErr(symbolName);
                printErr(" is already defined\n");
                return false;               
            }
            /*check that the symbol name is not used name (EX: r0, mov)*/
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
        printErr("can not allocate space to the new symbol\n");
        return false;
    }
    
    return true;
}
Symbol* getSymbolByName(char* symbolName){
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
                return &(symbolPtr -> value);               
            }
            symbolPtr = symbolPtr -> next;
        }
    }
    return NULL;
}
int getSymbolRefrenceByName(char* symbolName){
    Symbol *symbolPtr = getSymbolByName(symbolName);
    if(symbolPtr != NULL)
        return symbolPtr -> refrence;
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

void updateSymbolTableRefrences(int icPointer){
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
                symbolNodePtr -> value.refrence = currentSymbol.refrence + icPointer;
                
            }
            symbolNodePtr = symbolNodePtr -> next;
        }
    }
}

void printTable(){
    SymbolNode *symbolPtr;
    
    if(symbolTable != NULL)
    {
        /*check if symbol is not in the table already*/
        symbolPtr = symbolTable;
        while(symbolPtr != NULL)
        {
            printf("%s|%d|%d|%d\n",symbolPtr -> value.name,symbolPtr -> value.refrence,symbolPtr -> value.isExternal,
            symbolPtr -> value.commandType);
            symbolPtr = symbolPtr -> next;
        }
    }
}