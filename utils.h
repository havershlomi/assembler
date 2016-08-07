#include <string.h>
#include <stdlib.h>
#include<stdarg.h>
#include<stdio.h>
#include <ctype.h>


#define ACTION_MAX_LENGTH 4
#define INVALID -1
#define LINE_LENGTH 80
#define WORD_SIZE 15
#define OCTAL_WORD_SIZE 15
#define COLLECTION_SIZE 1000

/*enums*/

enum {false = 0,true = 1};
enum instructions{data = 0, string = 1, entry = 2, external = 3, invalidInstruction = -1};
enum commandTypes{ actionCommand = 1, instructionCommand = 2, none = 0};
enum operandType{instant = 0, direct = 1, dynamic = 2, directRegister = 3, invalidOperand = -1, notUsedOper = -2};
enum wordType { commandType = 1, registerValueType = 2, regularValueType = 3, base8Type = 4, dataType = 5};
enum ERA { absolute = 0, externalData = 1, relocatable = 2};

/* prints all the errors to stderr */
void printErr(char * format,...);

/* gets a number and the number if bits to cut from it */
int cutByBits(const int number, int from, int to);

/* return a value indication if there was errors during the assembelr iterrations */
int hasErrorInInput();

/* set the error flag back to false */
void resetErrorFlag();

/*  gets a string 
return the string without the white spaces from the end abd beginning */
char *trimWhiteSpace(char *str);
