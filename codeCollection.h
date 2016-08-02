#include "dataStructure.h"

/*return action object from the validaAction array
if not exist return null*/
Action* getActionByName(const char* name); 

int addAction();

int getOperandType(const char* oper);

int isValidBlockAddressTypeForAction(int sourcingType, int* validBLA);

int getActionRefrenceinMemory(int blaSrc, int blaDest);