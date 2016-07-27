#include "utils.h"
#include "parser.h"


int getInstructionType(const char* instruction){
    if(strcmp(instruction,"data") == 0)
        return data;
    else if(strcmp(instruction,"string") == 0)
        return string;
    else if(strcmp(instruction,"extern") == 0)
        return externl;
    else if(strcmp(instruction,"entry") == 0)
        return entry;
    else
        return invalidInstruction;
}