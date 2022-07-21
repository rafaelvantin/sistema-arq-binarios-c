run:
	./build/exec

reset:
	rm build/*

zip:
	zip -r trabalho.zip functions/* headers/* *.c Makefile

all: build_file main.o aux_functions.o tipo1.o tipo2.o indice.o create_bin_file.o print_all.o search.o logic_delete.o insert_into.o update.o
	gcc -o build/exec build/*.o -ggdb3

new_bin:
	cp -a .test/arquivos/trabalho3/antes/. .test/arquivos/trabalho3/usar/

build_file:
	mkdir -p build

main.o: main.c
	gcc -c main.c -o build/main.o -ggdb3

tipo1.o: tipo1.c headers/tipo1.h
	gcc -c tipo1.c -o build/tipo1.o -ggdb3

tipo2.o: tipo2.c headers/tipo2.h
	gcc -c tipo2.c -o build/tipo2.o -ggdb3

aux_functions.o: aux_functions.c headers/aux_functions.h
	gcc -c aux_functions.c -o build/aux_functions.o -ggdb3

indice.o: indice.c headers/indice.h
	gcc -c indice.c -o build/indice.o -ggdb3

create_bin_file.o: functions/create_bin_file.c headers/create_bin_file.h
	gcc -c functions/create_bin_file.c -o build/create_bin_file.o -ggdb3
	
print_all.o: functions/print_all.c headers/print_all.h
	gcc -c functions/print_all.c -o build/print_all.o -ggdb3

search.o: functions/search.c headers/search.h
	gcc -c functions/search.c -o build/search.o -ggdb3

logic_delete.o: functions/logic_delete.c headers/logic_delete.h
	gcc -c functions/logic_delete.c -o build/logic_delete.o -ggdb3

insert_into.o: functions/insert_into.c headers/insert_into.h
	gcc -c functions/insert_into.c -o build/insert_into.o -ggdb3

update.o: functions/update.c headers/update.h
	gcc -c functions/update.c -o build/update.o -ggdb3
