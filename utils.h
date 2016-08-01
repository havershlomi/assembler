#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define LABAL_MAX_LENGTH 30
#define KNRM  "\x1B[34m"

enum {false,true};
enum instructions{data,string,entry,externl,invalidInstruction};
enum sentenceType{action,instruction};
enum operandType{instant = 0, direct = 1, dynamic = 2, directRegister = 3, invalidOperand = -1};
void printErr(const char* errMessage);