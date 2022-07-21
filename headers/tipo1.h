#ifndef TIPO_1
#define TIPO_1

typedef struct tipo1 tipo1_t;

struct tipo1
{
    char removido;
    int id;
    int ano;
    int qtt;
    char sigla[2];
    char *cidade;
    char *marca;
    char *modelo;
    int prox;
    int tamCidade;
    int tamMarca;
    int tamModelo;
};

typedef struct cabecalho1 cabecalho1_t;

tipo1_t le_registro_dados_tipo1(FILE *fp);

void printa_registro_tipo1(tipo1_t registro);
int search_RRN_by_id(FILE *findex, int neededId);

int check_condicoes_sao_satisfeitas_tipo1(tipo1_t registro, char **nome_campos, char **valor_campos, int num_parametros);

#endif 