#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"

#include "../headers/aux_functions.h"

/*
* @brief Dado o offset e o fp, remove o arquivo e atualiza dados do cabeçalho
*/
void remove_registro_tipo1(FILE *fp, long offset_removido_reg)
{
    long old_offset = ftell(fp);
    fseek(fp, offset_removido_reg, SEEK_SET);

    char removido = '1';
    fwrite(&removido, sizeof(char), 1, fp);

    int prox = get_topo_tipo1(fp);
    fwrite(&prox, sizeof(int), 1, fp);

    int rrn = (offset_removido_reg - END_HEADER1_OFFSET) / REGISTER_SIZE;
    set_topo_tipo1(fp, rrn);

    set_nroRegRem_tipo1(fp, get_nroRegRem_tipo1(fp) + 1);

    fseek(fp, old_offset, SEEK_SET);
}

/*
* @brief Dado os inputs e os file pointers, avalia o registro a ser removido e chama remove_registro_tipo1() 
*/
void remove_registro_por_parametro_tipo1(FILE *fp, FILE *findex, char ***nome_campos, char ***valor_campos, int *num_parametros, int num_remocoes)
{
    for (int i = 0; i < num_remocoes; i++)
    {
        tipo1_t registro;
        int tamanho;

        // Procura o RRN por id
        int rrn = -1;

        for (int j = 0; j < num_parametros[i]; j++)
        {
            if (strcmp(nome_campos[i][j], "id") == 0)
                rrn = search_RRN_by_id(findex, atoi(valor_campos[i][j]));
        }

        // Se achou vai para o RRN, senao vai no primeiro registro
        if(rrn != -1)
            fseek(fp, END_HEADER1_OFFSET + (rrn * REGISTER_SIZE), SEEK_SET);
        else
            fseek(fp, END_HEADER1_OFFSET, SEEK_SET);

        // Roda pelos registros  
        while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
        {
            if (registro.removido == '1')
            {
                fseek(fp, REGISTER_SIZE - 1, SEEK_CUR);
                continue;
            }

            fseek(fp, -1, SEEK_CUR);
            long offset_removido_reg = ftell(fp);

            registro = le_registro_dados_tipo1(fp);

            int remove = check_condicoes_sao_satisfeitas_tipo1(registro, nome_campos[i], valor_campos[i], num_parametros[i]);

            if (remove == 1)
                remove_registro_tipo1(fp, offset_removido_reg);

            free(registro.cidade);
            free(registro.marca);
            free(registro.modelo);

            // Se foi buscado rrn pelo id => só olha pra um registro
            if(rrn != -1)
                break;
        }
    }
}


/*
* @brief Dado fp retorna o prox do cabecalho
*/
long long get_prox_tipo2(FILE *fp)
{
    long old_offset = ftell(fp);
    fseek(fp, 5L, SEEK_CUR);

    long long prox;
    fread(&prox, sizeof(long long), 1, fp);

    fseek(fp, old_offset, SEEK_SET);
    return prox;
}

/*
* @brief Dado fp no inicio de um reg retorna o tamanhoRegistro
*/
int get_tamanhoRegistro_tipo2(FILE *fp)
{
    long old_offset = ftell(fp);
    fseek(fp, 1L, SEEK_CUR);

    int tamanhoRegistro;
    fread(&tamanhoRegistro, sizeof(int), 1, fp);

    fseek(fp, old_offset, SEEK_SET);
    return tamanhoRegistro;
}

/*
* @brief Remove o registro, adicionando seu offset na lista ordenada
*/
void remove_registro_tipo2(FILE *fp, long long offset_removido_reg)
{
    long long old_offset = ftell(fp);

    // Muda removido para '1'
    fseek(fp, offset_removido_reg, SEEK_SET);
    char removido = '1';
    fwrite(&removido, sizeof(char), 1, fp);

    int size_reg_removido;
    fread(&size_reg_removido, sizeof(int), 1, fp);

    // Anterior e topo vars auxiliares  
    long long anterior = -1;
    long long prox = get_topo_tipo2(fp);
    fseek(fp, prox, SEEK_SET);

    // Insere na lista de ordem decrescente
    // Enquanto nao for fim da lista (prox != -1) e tamanho(reg) < tamanho(prox reg)
    while (prox != -1 && size_reg_removido < get_tamanhoRegistro_tipo2(fp))
    {
        anterior = prox;
        prox = get_prox_tipo2(fp);
        fseek(fp, prox, SEEK_SET);
    }

    // Atualiza prox do reg_removido
    if (prox != -1)
    {
        fseek(fp, offset_removido_reg + 5, SEEK_SET);
        fwrite(&prox, sizeof(long long), 1, fp);
    }

    // Atualiza prox do registro imediatamente anterior ao removido
    // Se for o maior (anterior = -1) atualiza topo
    if (anterior == -1)
    {
        set_topo_tipo2(fp, offset_removido_reg);
    }
    else
    {
        fseek(fp, anterior + 5, SEEK_SET);
        fwrite(&offset_removido_reg, sizeof(long long), 1, fp);
    }

    // Atualiza nroRegRem no cabecalho
    set_nroRegRem_tipo2(fp, get_nroRegRem_tipo2(fp) + 1);

    fseek(fp, old_offset, SEEK_SET);
}


/*
* @brief Avalia se registro satisfaz as condicoes e se sim remove com remove_registro_tipo2()
*/
void remove_registro_por_parametro_tipo2(FILE *fp, FILE *findex, char ***nome_campos, char ***valor_campos, int *num_parametros, int num_remocoes)
{
    for (int i = 0; i < num_remocoes; i++)
    {
        tipo2_t registro;
        int tamanho;

        // Procura o offset por id
        long long byteoffset = -1;

        for (int j = 0; j < num_parametros[i]; j++)
        {
            if (strcmp(nome_campos[i][j], "id") == 0)
                byteoffset = search_byteoffset_by_id(findex, atoi(valor_campos[i][j]));
        }

        // Se achou o offset do registro pula para ele, senao vai do primeiro registro
        if(byteoffset != -1)
            fseek(fp, byteoffset, SEEK_SET);
        else
            fseek(fp, END_HEADER2_OFFSET, SEEK_SET);

        // Roda pelos registros
        while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
        {
            fread(&registro.tamanhoRegistro, sizeof(int), 1, fp);

            if (registro.removido == '1')
            {
                fseek(fp, registro.tamanhoRegistro, SEEK_CUR);
                continue;
            }

            // Volta para o começo do registro
            fseek(fp, -5, SEEK_CUR);

            long long offset_removido_reg = ftell(fp);

            registro = le_registro_dados_tipo2(fp);

            int remove = check_condicoes_sao_satisfeitas_tipo2(registro, nome_campos[i], valor_campos[i], num_parametros[i]);

            if (remove == 1)
                remove_registro_tipo2(fp, offset_removido_reg);

            // Free's

            free(registro.cidade);
            free(registro.marca);
            free(registro.modelo);

            if (byteoffset != -1)
                break;
        }
    }
}
