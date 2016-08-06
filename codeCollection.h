#include "dataStructure.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#define IC_START_POSITION 100

/* get action name 
 return the action struct with the action properties*/
Action* getActionByName(const char* name);

/* get an operand string
return the enum value indicating the valid operand type */
int getOperandType(const char* oper);

/* get src and dest operand enum values
increment the ic counter
return the ic for the beginning of the action*/
int getActionRefrenceinMemory(int blaSrc, int blaDest);

/* get the ic current pointer */
int getICPointer();

/* get action and attributes check if they are valid and 
	return the action ic position */
int calculateActionIcPointer(char *action,char *actionAttr);

/* get action and attributes check if they are valid and insert them to the code collection*/
void addActionToCodeCollection(char *action, char *actionAttr);

/* get address enum type and a list of valid addressing types
return true of addressing is allowes false otherwise */
int isValidAddressTypeForAction(int sourcingType, int* validBLA);

/* print the entire code collection to the .ob file */
void printCodeCollection();

/* get addressing type and the data and the data word to the code collection*/
void addDataWordToCollection(int addressingType, char* value);

/* get a word and add it to the collection 
returns the ic position of this word*/
int addToCollection(WordDef * wordDef, int wordType);

/* get an action word and add it to the collection 
returns the ic position of this word*/
int addActionWord(int actionIndex, int numOfOperands,int srcAddressingType,int destAddressingType);

/* get command properties
return a refrence to the command word */
WordDef* createCommandWord(int opCode, int group, int src, int dest, int Era);

/* get src and dest addressing types and data word 
and crete a register word.
if they are both register type put them in the same word
return refrence to this word */
WordDef* createRegisterWord(int srcAdressType, char* srcWord, int destAdressType, char* destWord);

/* get value and create an instant data word
return refrence to this word */
WordDef* createInstantWord(char* value);

/* get value and create a direct data word
return refrence to this word */
WordDef* createDirectWord(char* value);

/* get value and create a dynamic data word
return refrence to this word */
WordDef* createDynamicWord(char* value);

/* get decimal representation of the ic location of a command */
unsigned int getCommandICPositionByRefrenceID(int refrenceID);

/* reset the ic counter */
void resetIc();

/* remove all the words from the data collection */
void cleanCodeCollection();