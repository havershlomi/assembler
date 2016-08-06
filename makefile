
assembler : assembler.c utils.o  sharedCollections.o parser.o codeCollection.o dataCollection.o symbolsTable.o fileHandler.o binaryConvertor.o 
	gcc -g -ansi -Wall -pedantic assembler.c utils.o  sharedCollections.o parser.o codeCollection.o dataCollection.o symbolsTable.o fileHandler.o binaryConvertor.o -o assembler

parser.o : parser.c 
	gcc -c -ansi -Wall -pedantic parser.c -o parser.o
codeCollection.o :  codeCollection.c
	gcc -c -ansi -Wall -pedantic codeCollection.c -o codeCollection.o
fileHandler.o : fileHandler.c
	gcc -c -ansi -Wall -pedantic fileHandler.c -o fileHandler.o
sharedCollections.o : sharedCollections.c
	gcc -c -ansi -Wall -pedantic sharedCollections.c -o sharedCollections.o
binaryConvertor.o : binaryConvertor.c
	gcc -c -ansi -Wall -pedantic binaryConvertor.c -o binaryConvertor.o
symbolsTable.o : symbolsTable.c 
	gcc -c -ansi -Wall -pedantic symbolsTable.c -o symbolsTable.o
dataCollection.o : dataCollection.c 
	gcc -c -ansi -Wall -pedantic dataCollection.c -o dataCollection.o
utils.o : utils.c
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
