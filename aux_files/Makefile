run: exec
	./exec

all: index.o usestrings.o lista.o hash.o
	gcc -o exec *.o -ggdb3

index.o: index.c
	gcc -c index.c -ggdb3
	
usestrings.o: usestrings.c usestrings.h
	gcc -c usestrings.c -ggdb3

lista.o: lista.c lista.h
	gcc -c lista.c -ggdb3

hash.o: hash.c hash.h
	gcc -c hash.c -ggdb3
	
