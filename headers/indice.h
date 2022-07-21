// Indice comum

int get_prox_id_tipo1(FILE *fp);

int get_prox_id_tipo2(FILE *fp, long long *byteoffset);

void grava_registro_indice_tipo1(FILE *fp, FILE *findex);
void grava_registro_indice_tipo2(FILE *fp, FILE *findex);

// Arvore-B

void define_cabecalho_arvb(FILE *findex, char* tipo_arquivo);
void grava_registro_arvb_tipo1(FILE *fp, FILE *findex);
void grava_registro_arvb_tipo2(FILE *fp, FILE *findex);

int search_rrn_by_id_arvb(FILE *findex, int RRN, int to_search_KEY);
long search_byteoffset_by_id_arvb(FILE *findex, int RRN, int to_search_KEY);
