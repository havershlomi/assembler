parserTest : parserTest.c utils.o  sharedCollections.o parser.o codeCollection.o dataCollection.o symbolsTable.o fileHandler.o binaryConvertor.o 
	gcc -g -ansi -Wall -pedantic parserTest.c utils.o  sharedCollections.o parser.o codeCollection.o dataCollection.o symbolsTable.o fileHandler.o binaryConvertor.o -o parserTest
binaryTest : binaryTest.c utils.o 
	gcc -g -ansi -Wall -pedantic binaryTest.c utils.o -o binaryTest
parser.o : ../parser.c 
	gcc -c -ansi -Wall -pedantic ../parser.c -o parser.o
codeCollection.o :  ../codeCollection.c
	gcc -c -ansi -Wall -pedantic ../codeCollection.c -o codeCollection.o
	
fileHandler.o : ../fileHandler.c
	gcc -c -ansi -Wall -pedantic ../fileHandler.c -o fileHandler.o

sharedCollections.o : ../sharedCollections.c
	gcc -c -ansi -Wall -pedantic ../sharedCollections.c -o sharedCollections.o

binaryConvertor.o : ../binaryConvertor.c
	gcc -c -ansi -Wall -pedantic ../binaryConvertor.c -o binaryConvertor.o

symbolsTableTest : symbolsTableTest.c symbolsTable.o utils.o
	gcc -g -ansi -Wall -pedantic symbolsTableTest.c symbolsTable.o utils.o -o symbolsTableTest
symbolsTable.o : ../symbolsTable.c 
	gcc -c -ansi -Wall -pedantic ../symbolsTable.c -o symbolsTable.o
dataCollection.o : ../dataCollection.c 
	gcc -c -ansi -Wall -pedantic ../dataCollection.c -o dataCollection.o 
utils.o : ../utils.c
	gcc -c -ansi -Wall -pedantic ../utils.c -o utils.o
