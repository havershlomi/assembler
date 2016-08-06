#include "utils.h"
#include "parser.h"
#include "codeCollection.h"
#include "dataCollection.h"
#include "symbolsTable.h"
#include "fileHandler.h"
#include "binaryConvertor.h"

/* clean all the assembler collections */
void cleanAllCollections();

/* print the top two liens of the ob file */
void printObjFileHeader();