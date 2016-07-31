typedef struct symbol {
    char name[30];
    int refrence;
    int type;
    int isExternal;
} Symbol;

typedef struct symbolNode {
    Symbol value; 
    struct symbolNode* next;
} SymbolNode;  

SymbolNode *symbolTable;

typedef struct action {
    char *name;
	int binaryIndex;
    int sourceOper[4];
    int destinationOper[4];
} Action;

typedef union word {
    struct data{
        unsigned int cell:15;
    } data;
} Word;