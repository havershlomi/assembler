
void parseDataInstruction(char *dataStr, const int hasLabel);
void parseStringInstruction(char *dataStr, const int hasLabel);
int getInstructionType(const char* instruction);
void parse(FILE* input);