#include "lista.h"

lista_t** create_hash_table(int m);
void destroy_hash_table(lista_t** t, int m);

void insert_hash(lista_t** t, char* key, int m);
void remove_hash(lista_t** t, char* key, int m);

void get(lista_t** t, int pos);
void check(lista_t** t, char* key, int m);