int get_tamanho_string(char *string);
tipo2_t update_registro_tipo2(tipo2_t registro, char **nome_campos_update, char **valor_campos_update, int qtt_campos_updates);
tipo1_t update_registro_tipo1(tipo1_t registro, char **nome_campos_update, char **valor_campos_update, int qtt_campos_updates);
void atualiza_registro_por_parametro_tipo1(FILE *fp, FILE *findex, char ***nome_campos_procura, char ***valor_campos_procura, char ***nome_campos_update,
                                           char ***valor_campos_update, int *qtt_campos_busca, int *qtt_campos_updates, int num_updates);
void atualiza_registro_por_parametro_tipo2(FILE *fp, FILE *findex, char ***nome_campos_procura, char ***valor_campos_procura, char ***nome_campos_update,
                                           char ***valor_campos_update, int *qtt_campos_busca, int *qtt_campos_updates, int num_updates);
