#include "dataStructure.h"
#define IC_START_POSITION 100

/*return action object from the validaAction array
if not exist return null*/
Action* getActionByName(const char* name); 

int addAction();

int getOperandType(const char* oper);

int isValidBlockAddressTypeForAction(int sourcingType, int* validBLA);

int getActionRefrenceinMemory(int blaSrc, int blaDest);

int getICPointer();