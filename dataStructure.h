typedef struct symbol {
    char name[30];
    int refrence;
} Symbol;

typedef struct symbolNode {
    Symbol value; 
    struct symbolNode* next;
} SymbolNode;  

SymbolNode *symbolTable;

typedef struct action {
    char name[4];
	int binaryIndex;
    int sourceOper[4];
    int destinationOper[4];
} Action;