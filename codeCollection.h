#include "dataStructure.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#define IC_START_POSITION 100

/*return action object from the validaAction array
if not exist return null*/
Action* getActionByName(const char* name); 

int addActionToCodeCollection(char *action,char *actionAttr);

int getOperandType(const char* oper);

int isValidAddressTypeForAction(int sourcingType, int* validBLA);

int getActionRefrenceinMemory(int blaSrc, int blaDest);

int getICPointer();

void resetIc();

void printCodeCollection();
void addDataWordToCollection(int addressingType, char* value);
int addToCollection(WordDef * wordDef, int wordType);

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era);
WordDef* getRegisterWord(int srcAdressType, char* srcWord, int destAdressType, char* destWord);
WordDef* getInstantWord(char* value);
WordDef* getDirectWord(char* value);
WordDef* getDynamicWord(char* value);
