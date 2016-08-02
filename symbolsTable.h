#define SYMBOL_NOT_FOUND -1;
/*checks if a the symbol is in the list
true - print an error
false - add it to the list
return true if succeeded false otherwise*/
int tryAddSymbol(Symbol* symbol);
/*find symbol refrence number in the table using his name*/
int getSymbolRefrenceByName(char* symbolName);
/*clear all the allocations the table made and reset table*/
void disposeSymbolsTable();

void printTable();