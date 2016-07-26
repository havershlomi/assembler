typedef struct symbol {
    char name[30];
    int refrence;
} Symbol;

typedef struct symbolNode {
    Symbol value; 
    struct symbolNode* next;
} SymbolNode;  


SymbolNode *symbolTable;