typedef struct symbol {
    char* name;
    int refrence;
} Symbol;

typedef struct symbolNode {
    Symbol value; 
    struct symbolNode* next;
} SymbolNode;  


SymbolNode *symbolTable;