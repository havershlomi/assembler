parserTest : parserTest.c utils.o
	gcc -g -ansi -Wall -pedantic parserTest.c utils.o -o parserTest
symbolsTableTest : symbolsTableTest.c symbolsTable.o utils.o
	gcc -g -ansi -Wall -pedantic symbolsTableTest.c symbolsTable.o utils.o -o symbolsTableTest
symbolsTable.o : ../symbolsTable.c 
	gcc -c -ansi -Wall -pedantic ../symbolsTable.c -o symbolsTable.o 
utils.o : ../utils.c
	gcc -c -ansi -Wall -pedantic ../utils.c -o utils.o
