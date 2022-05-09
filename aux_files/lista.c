#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "lista.h"

typedef struct no no_t;

struct no 
{
    char* info;
    no_t* prox;
};

struct lista
{
    no_t* ini;
    no_t* fim;
    int tam;
};

lista_t* cria()
{
    lista_t *l;

    l = malloc(sizeof(lista_t));

    assert(l != NULL);

    l->ini = NULL;
    l->fim = NULL;
    l->tam = 0;

    return l;
}

void libera(lista_t* l)
{
    if(l != NULL)
    {
        no_t* p = l->ini;

        while(p != NULL)
        {
            l->ini = p->prox;
            free(p->info);
            free(p);
            p = l->ini;
        }

        free(l);
    }
}

int insere(lista_t* l, char* x)
{
    assert(l != NULL);


    if(exists(l, x) == 1)
        return -1;

    no_t *p = malloc(sizeof(no_t));
    p->info = malloc(sizeof(char) * (strlen(x) + 1));
    strcpy(p->info, x);
 
    p->prox = l->ini;
    l->ini = p;

    if(l->fim == NULL)
        l->fim = p;

    l->fim = p;

    l->tam++;
    
    return 1;
}



int retira(lista_t* l, char* x)
{
     no_t *p = l->ini;
     no_t *anterior = NULL;

     while(p != NULL)
     {
         if(strcmp(p->info, x) == 0)
         {
            if(p == l->ini)
                l->ini = l->ini->prox;
            else if(p == l->fim)
            {
                if(anterior != NULL)
                {
                    l->fim = anterior;
                    l->fim->prox = NULL;
                }
                else
                    l->fim = NULL;
            }
            else
                anterior->prox = p->prox;

            free(p->info);
            free(p);
            l->tam--;

            return 1;
         }

         anterior = p;
         p = p->prox;
     }

     return 0;
}

int tamanho(lista_t* l)
{
    assert(l != NULL);

    return l->tam;
}

int exists(lista_t* l, char* x)
{
    assert(l != NULL);

    no_t* p = l->ini;

    while(p != NULL)
    {
        if(strcmp(p->info,x) == 0)
            return 1;
        p = p->prox;
    }

    return 0;
}

void imprime(lista_t* l)
{
    no_t *aux = l->ini;

    while(aux != NULL)
    {
        printf("%s ", aux->info);
        aux = aux->prox;
    }
    
    printf("\n");
}
