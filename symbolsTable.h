#define SYMBOL_NOT_FOUND -1
/*checks if a the symbol is in the list
true - print an error
false - add it to the list
return true if succeeded false otherwise*/
int tryAddSymbol(Symbol* symbol);

/*find symbol refrence number in the table using his name*/
int getSymbolRefrenceByName(char* symbolName);

/*find symbol in the table using his name*/
Symbol* getSymbolByName(char* symbolName);

/*clear all the allocations the table made and reset table*/
void disposeSymbolsTable();

/* get the properties of symbol and return a symbol object */
Symbol* createSymbol(char *label, int refrence, int commandType, int isExternal);

/* after  the first iteration update the ic counter of all the data */
void updateSymbolTableRefrences(int icPointer);