/*
* SCC0201 - Introdução à Ciência de Computação II
* Projeto 4 - Tabela Hash 
* 
* Rafael Sartori Vantin
* nUSP: 12543353
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#include "usestrings.h"

enum {ADD, DEL, GET, CHECK};

int get_operacao(char *texto)
{
    if(strcmp(texto, "add") == 0)
        return ADD;
    if(strcmp(texto, "del") == 0)
        return DEL;
    if(strcmp(texto, "get") == 0)
        return GET;
    if(strcmp(texto, "check") == 0)
        return CHECK;
}

int main()
{
    int size_table;
    scanf("%d\n", &size_table);
    
    int num_op;
    scanf("%d\n", &num_op);
    
    lista_t** hash_table = create_hash_table(size_table);

    for(int i = 0; i < num_op; i++)
    {
        char *line = readline();
        int countWords;
        char **splittedLine = split_line(&countWords, line);

        switch (get_operacao(splittedLine[0]))
        {
            case ADD:
                insert_hash(hash_table, splittedLine[1], size_table);
                break;

            case DEL:
                remove_hash(hash_table, splittedLine[1], size_table);
                break;

            case GET:
                get(hash_table, atoi(splittedLine[1]));
                break;
                
            case CHECK:
                check(hash_table, splittedLine[1], size_table);
                break;
        
            default:
                break;
        }


        for(int i = 0; i <= countWords; i++)
            free(splittedLine[i]);
        
        free(splittedLine);
        free(line);
    }

    destroy_hash_table(hash_table, size_table);

    return 0;
}