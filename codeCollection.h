#include "dataStructure.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#define IC_START_POSITION 100

/*return action object from the validaAction array
if not exist return null*/
Action* getActionByName(const char* name); 

int getOperandType(const char* oper);
int getActionRefrenceinMemory(int blaSrc, int blaDest);
int getICPointer();

int getActionAddressingType(char *action,char *actionAttr);
int addActionToCodeCollection(char *action,char *actionAttr);

int isValidAddressTypeForAction(int sourcingType, int* validBLA);

void resetIc();

void printCodeCollection();
void addDataWordToCollection(int addressingType, char* value);
int addToCollection(WordDef * wordDef, int wordType);
int addActionWord(int actionIndex, int numOfOperands,int srcAddressingType,int destAddressingType);

WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era);
WordDef* createRegisterWord(int srcAdressType, char* srcWord, int destAdressType, char* destWord);
WordDef* createInstantWord(char* value);
WordDef* createDirectWord(char* value);
WordDef* createDynamicWord(char* value);

unsigned int getWordNumberByRefrenceID(int refrenceID);
