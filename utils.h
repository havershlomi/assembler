#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define LABAL_MAX_LENGTH 30
#define KNRM  "\x1B[34m"
#define INVALID -1
#define LINE_LENGTH 80


enum {false = 0,true = 1};
enum instructions{data = 0,string = 1,entry = 2,externl = 3,invalidInstruction = -1};
enum commandType{actionCommand = 1,instructionCommand = 2,none = 0};
enum operandType{instant = 0, direct = 1, dynamic = 2, directRegister = 3, invalidOperand = -1, notUsedOper = -2};
void printErr(const char* errMessage);