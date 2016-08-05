#include <string.h>
#include <stdlib.h>
#include<stdarg.h>
#include<stdio.h>
#include <ctype.h>


#define LABAL_MAX_LENGTH 30
#define KNRM  "\x1B[34m"
#define INVALID -1
#define LINE_LENGTH 80


enum {false = 0,true = 1};
enum instructions{data = 0, string = 1, entry = 2, external = 3, invalidInstruction = -1};
enum commandTypes{ actionCommand = 1, instructionCommand = 2, none = 0};
enum operandType{instant = 0, direct = 1, dynamic = 2, directRegister = 3, invalidOperand = -1, notUsedOper = -2};
enum wordType { commandType = 1, registerValueType = 2, regularValueType = 3, base8Type = 4, dataType = 5};
enum ERA { absolute = 0, externalData = 1, relocatable = 2};
void printErr(char * format,...);
int cutByBits(const int number, int from, int to);
void int2bin(const int);
