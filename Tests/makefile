parserTest : parserTest.c utils.o parser.o codeCollection.o dataStructure.o
	gcc -g -ansi -Wall -pedantic parserTest.c utils.o parser.o codeCollection.o dataStructure.o -o parserTest
parser.o : ../parser.c
	gcc -c -ansi -Wall -pedantic ../parser.c -o parser.o

codeCollection.o :  ../codeCollection.c
	gcc -c -ansi -Wall -pedantic ../codeCollection.c -o codeCollection.o
	
dataStructure.o : ../dataStructure.c
	gcc -c -ansi -Wall -pedantic ../dataStructure.c -o dataStructure.o

symbolsTableTest : symbolsTableTest.c symbolsTable.o utils.o
	gcc -g -ansi -Wall -pedantic symbolsTableTest.c symbolsTable.o utils.o -o symbolsTableTest
symbolsTable.o : ../symbolsTable.c 
	gcc -c -ansi -Wall -pedantic ../symbolsTable.c -o symbolsTable.o 
utils.o : ../utils.c
	gcc -c -ansi -Wall -pedantic ../utils.c -o utils.o
