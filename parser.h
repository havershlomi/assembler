
/* add the string to the data collection
    return the index of the first char */
int addStringInstructionToDC(char *dataStr);

/* add the data numbers to the data collection
return the index of the first number */
int addDataInstructionToDC(char *dataStr);

/* get the name of an instruction
    return the appropriate enum value  */
int getInstructionType(const char* instruction);

/* get the label name and check if it's valid
    return true or false */
int isValidLabel(char *label);

/* get the input file
read each line assert if every line is valid
save symbols to the symbols table 
and encode action statment */
void firstIteration(FILE* input);


/* get the input file
read each line assert if every line is valid
write to entry log file
add data to code collection  
 */
void secondIteration(FILE* input);
