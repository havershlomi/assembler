#define NOT_USED_DEFAULT_VALUE 5 
typedef struct symbol {
    char name[30];
    int icRefrence;
    int commandType;
    int isExternal;
    int dcRefrence;
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
    int destOper[4];
    unsigned int numOfOperands;
} Action;

typedef union word {
    /*Binary command |notUsed|Group|opCode|SRC|DEST|ERA|*/
    struct {
        unsigned int ERA:2;
        unsigned int dest:2;
        unsigned int src:2;
        unsigned int opCode:4;
        unsigned int group:2;
        unsigned int notUsed:3;
    } command;
    /*Register values |NOTUSED|DEST|SRC|ERA|*/
    struct {
        unsigned int ERA:2;
        int src:6;
        int dest:6;
        unsigned int notUsed:1;
    } registerValue;
    /*Value |VALUE|ERA|*/
    struct {
        unsigned int ERA:2;
        signed int value:13;
    } regularValue;
    /*Base8*/
    struct{
        unsigned int block1:3;
        unsigned int block2:3;
        unsigned int block3:3;
        unsigned int block4:3;
        unsigned int block5:3;
    } Base8;
} WordDef;

typedef struct{
    WordDef *word;
    int wordType;
} Word;
