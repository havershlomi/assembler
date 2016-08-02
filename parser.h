int getActionBLAddress(char *action,char *actionAttr, const int hasLabel);
int addStringInstructionToDC(char *dataStr);
int addDataInstructionToDC(char *dataStr);
int getInstructionType(const char* instruction);
int isValidLabel(char *label);
void firstMove(FILE* input);