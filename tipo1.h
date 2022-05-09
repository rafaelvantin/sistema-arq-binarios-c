typedef struct tipo1 tipo1_t;

struct tipo1
{
    int id;
    int ano;
    int qtt;
    char sigla[2];
    char *cidade;
    char *marca;
    char *modelo;
    char removido;
    int prox;
    int tamCidade;
    int tamMarca;
    int tamModelo;
};

typedef struct cabecalho1 cabecalho1_t;

int is_eof(FILE *fp);
void printa_registro(tipo1_t dados);

int get_proxRRN(FILE *fp);
tipo1_t busca_prox_registro(FILE *fp);
tipo1_t busca_registro_por_RRN(FILE *fp, int RRN);
void busca_e_printa_registro_por_parametro(FILE *fp, char **nome_campos, char **valor_campos, int num_parametros);

void inicializa_cabecalho(FILE *fp);
void grava_registro(FILE *fp, tipo1_t dados);

tipo1_t le_registro_csv(FILE *fp);

void atualiza_status(FILE *fp, char status);