#include "dataStructure.h"
#define IC_START_POSITION 100

/*return action object from the validaAction array
if not exist return null*/
Action* getActionByName(const char* name); 

int addActionToCodeCollection(char *action,char *actionAttr);

int getOperandType(const char* oper);

int isValidBlockAddressTypeForAction(int sourcingType, int* validBLA);

int getActionRefrenceinMemory(int blaSrc, int blaDest);

int getICPointer();

void resetIc();

void printCodeCollection();

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era);
void addToCollection(WordDef * wordDef, int wordType);