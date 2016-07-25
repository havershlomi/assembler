assembler : assembler.c symbolsTable.o utils.o
	gcc -g -ansi -Wall -pedantic assembler.c symbolsTable.o utils.o -o assembler
symbolsTable.o : symbolsTable.c 
	gcc -c -ansi -Wall -pedantic symbolsTable.c -o symbolsTable.o 
utils.o : utils.c
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
