#define END_HEADER1_OFFSET 182
#define END_HEADER2_OFFSET 190
#define END_HEADER_INDEX_OFFSET 1
#define END_HEADER_ARVB_TIPO1_OFFSET 45
#define END_HEADER_ARVB_TIPO2_OFFSET 57
#define NO_SIZE_ARVB_TIPO1 45
#define NO_SIZE_ARVB_TIPO2 57
#define NOT_FOUND -1

#define REGISTER_SIZE 97

void binarioNaTela(char *nomeArquivoBinario);
FILE *open_file(const char *filename, const char *mode, int checkFileConsistent);
int isTipo1(const char *tipo_arquivo);
int isTipo2(const char *tipo_arquivo);

int get_proxRRN_tipo1(FILE *fp);
void atualiza_proxRRN_tipo1(FILE *fp, int proxRRN);
int get_proxByteOffset(FILE *fp);
void atualiza_proxByteOffset(FILE *fp, long long proxByteOffset);
int get_topo_tipo1(FILE *fp);
void set_topo_tipo1(FILE *fp, int topo);
long long get_topo_tipo2(FILE *fp);
void set_topo_tipo2(FILE *fp, long long topo);
int get_nroRegRem_tipo1(FILE *fp);
void set_nroRegRem_tipo1(FILE *fp, int numRegRem);
int get_nroRegRem_tipo2(FILE *fp);
void set_nroRegRem_tipo2(FILE *fp, int numRegRem);

int getNoRaiz_arvb(FILE *findex);
void setNoRaiz_arvb(FILE *findex, int noRaiz);
int getProxRRN_arvb(FILE *findex);
void setProxRRN_arvb(FILE *findex, int proxRRN);
int getNroNos_arvb(FILE *findex);
void setNroNos_arvb(FILE *findex, int nroNos);


char *read_till_delimitor(FILE *fp);
char *read_and_count_till_delimitor(FILE *fp, int *count_words);
char *read_word(FILE *fp);
char *read_quotes(FILE *fp);
char *read_quotes2(FILE *fp);
char *read_and_count_quotes(FILE *fp, int *count_words);

int is_file_consistent(FILE *fp);
int is_eof(FILE *fp);
void atualiza_status(FILE *fp, char status);