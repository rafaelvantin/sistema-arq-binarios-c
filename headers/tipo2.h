#ifndef TIPO_2
#define TIPO_2

typedef struct tipo2 tipo2_t;

struct tipo2
{
    char removido;
    int tamanhoRegistro;
    long long prox;
    int id;
    int ano;
    int qtt;
    char sigla[2];
    int tamCidade;
    char *cidade;
    int tamMarca;
    char *marca;
    int tamModelo;
    char *modelo;
};

typedef struct cabecalho2 cabecalho2_t;

tipo2_t le_registro_dados_tipo2(FILE *fp);

void printa_registro_tipo2(tipo2_t registro);
long long search_byteoffset_by_id(FILE *findex, int neededId);

int check_condicoes_sao_satisfeitas_tipo2(tipo2_t registro, char **nome_campos, char **valor_campos, int num_parametros);

#endif