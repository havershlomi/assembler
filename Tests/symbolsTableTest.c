#include "../utils.h"
#include "../dataStructure.h"
#include "../symbolsTable.h"

int main(){


    Symbol symbol = {"shlomi",34};
    Symbol symbol2 = {"david",4};
    Symbol symbol3 = {"yer",5};

    if(tryAddSymbol(&symbol) == true){
        printf( "added : %s\n", symbol.name);
    }
    if(tryAddSymbol(&symbol2) == true){
        printf( "added : %s\n", symbol2.name);
    }   
    if(tryAddSymbol(&symbol3) == true){
        printf( "added : %s\n", symbol3.name);
    }
    if(tryAddSymbol(&symbol2) == true){
        printf( "added : %s\n", symbol2.name);
    }

    printf("%d\n",getSymbolRefrenceByName("shlomi"));

    return 0;
}