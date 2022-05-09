run: exec
	./exec

all: index.o aux_functions.o tipo1.o tipo2.o
	gcc -o exec *.o -ggdb3

index.o: index.c
	gcc -c index.c -ggdb3

tipo1.o: tipo1.c tipo1.h
	gcc -c tipo1.c -ggdb3

tipo2.o: tipo2.c tipo2.h
	gcc -c tipo2.c -ggdb3

aux_functions.o: aux_functions.c aux_functions.h
	gcc -c aux_functions.c -ggdb3