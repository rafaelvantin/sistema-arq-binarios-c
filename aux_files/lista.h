typedef struct lista lista_t;

lista_t* cria();

void libera(lista_t* l);

int insere(lista_t* l, char* x);

int retira(lista_t* l, char* x);

int tamanho(lista_t* l);

int exists(lista_t* l, char* x);

void imprime(lista_t* l);