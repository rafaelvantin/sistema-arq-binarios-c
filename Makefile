run: exec
	./exec

all: index.o 
	gcc -o exec *.o -ggdb3

index.o: index.c
	gcc -c index.c -ggdb3