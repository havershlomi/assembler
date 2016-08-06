#include "utils.h"


int main(){


    Symbol symbol = {"shlomi"};
    SymbolNode *newNode = (SymbolNode*)malloc(sizeof(SymbolNode));
    newNode -> value = &symbol;
       

    printf( "Book title : %s\n", symbol.name);    

    return 0;
}