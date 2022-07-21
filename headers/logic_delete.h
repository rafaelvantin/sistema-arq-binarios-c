void remove_registro_tipo1(FILE *fp, long offset_removido_reg);
void remove_registro_por_parametro_tipo1(FILE *fp, FILE *findex, char ***nome_campos, char ***valor_campos, int *num_parametros, int num_remocoes);
long long get_prox_tipo2(FILE *fp);
int get_tamanhoRegistro_tipo2(FILE *fp);
void remove_registro_tipo2(FILE *fp, long long offset_removido_reg);
void remove_registro_por_parametro_tipo2(FILE *fp, FILE *findex, char ***nome_campos, char ***valor_campos, int *num_parametros, int num_remocoes);
