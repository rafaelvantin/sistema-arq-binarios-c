#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"

long power(int x, int y)
{
    long result = 1;

    for(int i = 0; i < y; i++)
        result *= x;

    return result;
}

lista_t** create_hash_table(int m)
{
	lista_t** hash_table = malloc(sizeof(lista_t*) * m);
	
    for (int i = 0; i < m; i++)
        hash_table[i] = cria();
	
    return hash_table;
}

void destroy_hash_table(lista_t** t, int m)
{
    for(int i = 0; i < m; i++)
        libera(t[i]);

	free(t);
}

int hash_function(char* key, int m)
{
    long long result = 0;
    int x = 263;
    int p = 1000000007;

    for(int i = 0; i < strlen(key); i++)
    {
        long long temp = ((int)key[i] * power(x, i)) % p;
        result += temp;
    }

    return result % p % m;
}

void insert_hash(lista_t** t, char* key, int m)
{
    int pos = hash_function(key, m);

    if(!exists(t[pos], key))
        insere(t[pos], key);
}

void remove_hash(lista_t** t, char *key, int m)
{
	int pos = hash_function(key, m);
    retira(t[pos], key);
}

void get(lista_t** t, int pos)
{
    imprime(t[pos]);
}

void check(lista_t** t, char* key, int m)
{
    int pos = hash_function(key, m);
    
    if(exists(t[pos], key))
        printf("sim\n");
    else
        printf("não\n");
}