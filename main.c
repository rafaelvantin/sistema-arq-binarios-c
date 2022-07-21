/*
 * SCC0215 - Organização de Arquivos
 * Trabalho 2 - G12
 *
 * Integrantes:
 *  -> Matheus Giraldi Alvarenga (nUSP: 12543669) 50%
 *  -> Rafael Sartori Vantin (nUSP: 12543353) 50%
 *
 * O Trabalho foi modularizado em 4 arquivos:
 * - main.c que lida com os inputs do usuário e com as aberturas de arquivo
 * - tipo2.c que lida com a manipulação de dados, read/write e checagem de parâmetros nos arquivos do tipo 2
 * - tipo1.c que lida com a manipulação de dados, read/write e checagem de parâmetros nos arquivos do tipo 1
 * - aux_functions.c que lida com a leitura de strings e o binarioNaTela
 * - indice.c que lida com a escrita de índices no arquivo de índices e com inserção e pesquisa de árvore-B
 * - /functions que contém os arquivos .c de cada funcionalidade do projeto
 * - /headers que contém os arquivos .h de cada funcionalidade do projeto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/indice.h"

#include "headers/create_bin_file.h"
#include "headers/print_all.h"
#include "headers/search.h"
#include "headers/logic_delete.h"
#include "headers/insert_into.h"
#include "headers/update.h"

#include "headers/aux_functions.h"

#define START_CSV_DATA_OFFSET 60

#define CREATE_BIN_FILE 1
#define PRINT_ALL 2
#define SEARCH_BY_PARAMS 3
#define SEARCH_BY_RRN 4
#define CREATE_INDEX_FILE 5
#define LOGIC_DELETE 6
#define INSERTION 7
#define UPDATE 8
#define CREATE_ARVB_FILE 9
#define SEARCH_BY_ID 10
#define INSERTION_ARVB 11

int main()
{
    int operacao;
    int num_campos;
    char *tipo_arquivo, *nome_arquivo_entrada, *nome_arquivo_indice, *nome_arquivo_saida;
    char ***nome_campos_multireg, ***valor_campos_multireg;
    FILE *fp, *fcsv, *findex;

    scanf("%d%*c", &operacao);

    switch (operacao)
    {
    case CREATE_BIN_FILE:

        /* ------ OPEN FILES ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_saida = read_word(stdin);

        fcsv = open_file(nome_arquivo_entrada, "rb", 0);
        fp = open_file(nome_arquivo_saida, "wb+", 0);

        /* ------ READ FROM CSV, WRITE IN BINARY ------ */

        atualiza_status(fp, '0');

        fseek(fcsv, START_CSV_DATA_OFFSET, SEEK_SET);
        int proxRRN = 0;

        if (isTipo1(tipo_arquivo))
        {
            inicializa_cabecalho_tipo1(fp);

            while (!is_eof(fcsv))
            {
                tipo1_t reg_lido = le_registro_csv_tipo1(fcsv);
                grava_registro_tipo1(fp, reg_lido);
                proxRRN++;
            }

            atualiza_proxRRN_tipo1(fp, proxRRN);
        }
        if (isTipo2(tipo_arquivo))
        {
            inicializa_cabecalho_tipo2(fp);

            while (!is_eof(fcsv))
            {
                tipo2_t reg_lido = le_registro_csv_tipo2(fcsv);
                grava_registro_tipo2(fp, reg_lido);
                atualiza_proxByteOffset(fp, get_proxByteOffset(fp) + reg_lido.tamanhoRegistro + 5);
                proxRRN++;
            }
        }

        atualiza_status(fp, '1');

        /* ------ CLOSES, FREES & BINARIONATELA ------ */
        fclose(fcsv);
        fclose(fp);

        binarioNaTela(nome_arquivo_saida);

        free(tipo_arquivo);
        free(nome_arquivo_entrada);
        free(nome_arquivo_saida);

        break;

    case PRINT_ALL:

        /* ------ OPEN FILES ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);

        fp = open_file(nome_arquivo_entrada, "rb", 1);

        /* ------ CALL PRINT FUNCTION ------ */
        if (isTipo1(tipo_arquivo))
            printa_todos_registros_tipo1(fp);
        if (isTipo2(tipo_arquivo))
            printa_todos_registros_tipo2(fp);

        /* ------ CLOSES, FREES ------ */
        free(tipo_arquivo);
        free(nome_arquivo_entrada);
        fclose(fp);
        break;

    case SEARCH_BY_PARAMS:

        /* ------ OPEN FILES ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);

        scanf("%d%*c", &num_campos);
        char **nome_campos = malloc(num_campos * sizeof(char *));
        char **valor_campos = malloc(num_campos * sizeof(char *));

        for (int i = 0; i < num_campos; i++)
        {
            nome_campos[i] = read_word(stdin);
            valor_campos[i] = read_quotes(stdin);
        }

        fp = open_file(nome_arquivo_entrada, "rb", 1);

        /* ------ CALL SEARCH FUNCTION ------ */
        if (isTipo1(tipo_arquivo))
            busca_e_printa_registro_por_parametro_tipo1(fp, nome_campos, valor_campos, num_campos);
        if (isTipo2(tipo_arquivo))
            busca_e_printa_registro_por_parametro_tipo2(fp, nome_campos, valor_campos, num_campos);

        /* ------ CLOSES, FREES ------ */
        fclose(fp);
        free(tipo_arquivo);
        free(nome_arquivo_entrada);
        for (int i = 0; i < num_campos; i++)
        {
            free(nome_campos[i]);
            free(valor_campos[i]);
        }

        free(nome_campos);
        free(valor_campos);

        break;

    case SEARCH_BY_RRN:

        /* ------ OPEN FILES & PRINT BY RRN ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);

        int RRN;
        scanf("%d%*c", &RRN);

        fp = open_file(nome_arquivo_entrada, "rb", 1);

        if (isTipo1(tipo_arquivo))
            busca_e_printa_registro_por_RRN(fp, RRN);

        /* ------ CLOSES, FREES ------ */
        fclose(fp);
        free(tipo_arquivo);
        free(nome_arquivo_entrada);
        break;

    case SEARCH_BY_ID:
        /* ------ OPEN FILES & PRINT BY RRN ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_indice = read_word(stdin);

        int id;
        scanf("id %d%*c", &id);

        findex = open_file(nome_arquivo_indice, "rb", 1);
        fp = open_file(nome_arquivo_entrada, "rb", 1);

        int no_raiz;
        fseek(findex, 1, SEEK_SET);
        fread(&no_raiz, sizeof(int), 1, findex);

        if (isTipo1(tipo_arquivo))
        {
            int RRN = search_rrn_by_id_arvb(findex, no_raiz, id);
            busca_e_printa_registro_por_RRN(fp, RRN);
        }
        if (isTipo2(tipo_arquivo))
        {
            long long byteoffset = search_byteoffset_by_id_arvb(findex, no_raiz, id);
            busca_e_printa_registro_por_offset(fp, byteoffset);
        }

        /* ------ CLOSES, FREES ------ */
        fclose(fp);
        free(tipo_arquivo);
        free(nome_arquivo_entrada);
        free(nome_arquivo_indice);
        break;

    case CREATE_INDEX_FILE:
    case CREATE_ARVB_FILE:
        /* ------ OPEN FILES & PRINT BY RRN ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_indice = read_word(stdin);

        findex = open_file(nome_arquivo_indice, "wb+", 0);
        fp = open_file(nome_arquivo_entrada, "rb", 1);

        /* ------ READ FROM BINARY, WRITE IN INDEX ------ */

        atualiza_status(findex, '0');

        if (operacao == CREATE_INDEX_FILE)
        {
            if (isTipo1(tipo_arquivo))
                grava_registro_indice_tipo1(fp, findex);
            if (isTipo2(tipo_arquivo))
                grava_registro_indice_tipo2(fp, findex);
        }
        else if (operacao == CREATE_ARVB_FILE)
        {
            define_cabecalho_arvb(findex, tipo_arquivo);

            if (isTipo1(tipo_arquivo))
                grava_registro_arvb_tipo1(fp, findex);
            if (isTipo2(tipo_arquivo))
                grava_registro_arvb_tipo2(fp, findex);
        }

        atualiza_status(findex, '1');

        /* ------ CLOSES, FREES & BINARIONATELA ------ */

        fclose(fp);
        fclose(findex);

        binarioNaTela(nome_arquivo_indice);

        free(tipo_arquivo);
        free(nome_arquivo_indice);
        free(nome_arquivo_entrada);
        break;

    case LOGIC_DELETE:
        /* ------ OPEN FILES & PRINT BY RRN ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_indice = read_word(stdin);

        int num_remocoes;
        scanf("%d%*c", &num_remocoes);

        int *qtt_campos = malloc(num_remocoes * sizeof(int));
        nome_campos_multireg = malloc(num_remocoes * sizeof(char **));
        valor_campos_multireg = malloc(num_remocoes * sizeof(char **));

        for (int i = 0; i < num_remocoes; i++)
        {
            scanf("%d%*c", &qtt_campos[i]);

            nome_campos_multireg[i] = malloc(qtt_campos[i] * sizeof(char *));
            valor_campos_multireg[i] = malloc(qtt_campos[i] * sizeof(char *));

            for (int j = 0; j < qtt_campos[i]; j++)
            {
                nome_campos_multireg[i][j] = read_word(stdin);
                if (nome_campos_multireg[i][j][0] == '\0')
                    break;

                if (strcmp(nome_campos_multireg[i][j], "sigla") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "cidade") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "modelo") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "marca") == 0)
                {
                    valor_campos_multireg[i][j] = read_quotes2(stdin);
                }
                else
                {
                    valor_campos_multireg[i][j] = read_word(stdin);
                }
            }
        }

        findex = open_file(nome_arquivo_indice, "rb", 1);
        fp = open_file(nome_arquivo_entrada, "rb+", 1);

        atualiza_status(fp, '0');

        /* ------ Remove registro arquivo de dados ------ */

        if (isTipo1(tipo_arquivo))
            remove_registro_por_parametro_tipo1(fp, findex, nome_campos_multireg, valor_campos_multireg, qtt_campos, num_remocoes);
        if (isTipo2(tipo_arquivo))
            remove_registro_por_parametro_tipo2(fp, findex, nome_campos_multireg, valor_campos_multireg, qtt_campos, num_remocoes);

        /* ------ Refaz arquivo de índice ------ */
        fclose(findex);
        findex = open_file(nome_arquivo_indice, "wb", 0);
        atualiza_status(findex, '0');

        if (isTipo1(tipo_arquivo))
            grava_registro_indice_tipo1(fp, findex);
        if (isTipo2(tipo_arquivo))
            grava_registro_indice_tipo2(fp, findex);

        atualiza_status(findex, '1');
        atualiza_status(fp, '1');

        /* ------ CLOSES, FREES & BINARIONATELA ------ */

        fclose(fp);
        fclose(findex);

        binarioNaTela(nome_arquivo_entrada);
        binarioNaTela(nome_arquivo_indice);

        for (int i = 0; i < num_remocoes; i++)
        {
            for (int j = 0; j < qtt_campos[i]; j++)
            {
                free(nome_campos_multireg[i][j]);
                free(valor_campos_multireg[i][j]);
            }

            free(nome_campos_multireg[i]);
            free(valor_campos_multireg[i]);
        }
        free(nome_campos_multireg);
        free(valor_campos_multireg);

        free(qtt_campos);

        free(tipo_arquivo);
        free(nome_arquivo_indice);
        free(nome_arquivo_entrada);
        break;

    case INSERTION:
    case INSERTION_ARVB:
        /* ------ OPEN FILES & PRINT BY RRN ------ */
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_indice = read_word(stdin);

        int num_insercoes;
        scanf("%d%*c", &num_insercoes);

        findex = open_file(nome_arquivo_indice, "wb+", 0);
        fp = open_file(nome_arquivo_entrada, "rb+", 1);

        atualiza_status(findex, '0');
        atualiza_status(fp, '0');

        if (isTipo1(tipo_arquivo))
        {
            tipo1_t dados[num_insercoes];

            for (int i = 0; i < num_insercoes; i++)
            {
                int count = 0;

                scanf("%d", &dados[i].id);

                char *ano = read_word(stdin);
                if (strcmp(ano, "NULO") == 0)
                    dados[i].ano = -1;
                else
                {
                    dados[i].ano = atoi(ano);
                }

                char *qtt = read_word(stdin);
                if (strcmp(qtt, "NULO") == 0)
                    dados[i].qtt = -1;
                else
                {
                    dados[i].qtt = atoi(qtt);
                }

                char *sigla = read_quotes2(stdin);
                if (strcmp(sigla, "NULO") != 0)
                    strcpy(dados[i].sigla, sigla);
                else
                {
                    strcpy(dados[i].sigla, "$$");
                }

                char *cidade = read_and_count_quotes(stdin, &count);
                if (strcmp(cidade, "NULO") != 0)
                {
                    dados[i].cidade = malloc(sizeof(char) * (count + 1));
                    dados[i].tamCidade = count;
                    strcpy(dados[i].cidade, cidade);
                }
                else
                {
                    dados[i].tamCidade = 0;
                }
                count = 0;

                char *marca = read_and_count_quotes(stdin, &count);
                if (strcmp(marca, "NULO") != 0)
                {
                    dados[i].marca = malloc(sizeof(char) * (count + 1));
                    dados[i].tamMarca = count;
                    strcpy(dados[i].marca, marca);
                }
                else
                {
                    dados[i].tamMarca = 0;
                }
                count = 0;

                char *modelo = read_and_count_quotes(stdin, &count);
                if (strcmp(modelo, "NULO") != 0)
                {
                    dados[i].modelo = malloc(sizeof(char) * (count + 1));
                    dados[i].tamModelo = count;
                    strcpy(dados[i].modelo, modelo);
                }
                else
                {
                    dados[i].tamModelo = 0;
                }

                free(ano);
                free(qtt);
                free(sigla);
                free(cidade);
                free(marca);
                free(modelo);
            }

            for (int i = 0; i < num_insercoes; i++)
            {
                insert_into_tipo_1(fp, dados[i]);
            }

            if (operacao == INSERTION)
                grava_registro_indice_tipo1(fp, findex);
            if (operacao == INSERTION_ARVB)
            {
                define_cabecalho_arvb(findex, tipo_arquivo);
                grava_registro_arvb_tipo1(fp, findex);
            }
        }
        if (isTipo2(tipo_arquivo))
        {
            tipo2_t dados[num_insercoes];

            for (int i = 0; i < num_insercoes; i++)
            {
                int count = 0;
                dados[i].tamanhoRegistro = 22;

                scanf("%d", &dados[i].id);

                char *ano = read_word(stdin);
                if (strcmp(ano, "NULO") == 0)
                    dados[i].ano = -1;
                else
                {
                    dados[i].ano = atoi(ano);
                }

                char *qtt = read_word(stdin);
                if (strcmp(qtt, "NULO") == 0)
                    dados[i].qtt = -1;
                else
                {
                    dados[i].qtt = atoi(qtt);
                }

                char *sigla = read_quotes2(stdin);
                if (strcmp(sigla, "NULO") != 0)
                    strcpy(dados[i].sigla, sigla);
                else
                {
                    strcpy(dados[i].sigla, "$$");
                }

                char *cidade = read_and_count_quotes(stdin, &count);
                if (strcmp(cidade, "NULO") != 0)
                {
                    dados[i].cidade = malloc(sizeof(char) * (count + 1));
                    dados[i].tamCidade = count;
                    strcpy(dados[i].cidade, cidade);
                    dados[i].tamanhoRegistro += count + 5;
                }
                else
                {
                    dados[i].tamCidade = 0;
                }
                count = 0;

                char *marca = read_and_count_quotes(stdin, &count);
                if (strcmp(marca, "NULO") != 0)
                {
                    dados[i].marca = malloc(sizeof(char) * (count + 1));
                    dados[i].tamMarca = count;
                    strcpy(dados[i].marca, marca);
                    dados[i].tamanhoRegistro += count + 5;
                }
                else
                {
                    dados[i].tamMarca = 0;
                }
                count = 0;

                char *modelo = read_and_count_quotes(stdin, &count);
                if (strcmp(modelo, "NULO") != 0)
                {
                    dados[i].modelo = malloc(sizeof(char) * (count + 1));
                    dados[i].tamModelo = count;
                    strcpy(dados[i].modelo, modelo);
                    dados[i].tamanhoRegistro += count + 5;
                }
                else
                {
                    dados[i].tamModelo = 0;
                }

                free(ano);
                free(sigla);
                free(qtt);
                free(cidade);
                free(marca);
                free(modelo);
            }

            for (int i = 0; i < num_insercoes; i++)
            {
                insert_into_tipo_2(fp, dados[i]);
            }

            if (operacao == INSERTION)
                grava_registro_indice_tipo2(fp, findex);
            if (operacao == INSERTION_ARVB)
            {
                define_cabecalho_arvb(findex, tipo_arquivo);
                grava_registro_arvb_tipo2(fp, findex);
            }
        }

        atualiza_status(findex, '1');
        atualiza_status(fp, '1');

        /* ------ CLOSES, FREES & BINARIONATELA ------ */

        fclose(fp);
        fclose(findex);

        binarioNaTela(nome_arquivo_entrada);
        binarioNaTela(nome_arquivo_indice);

        free(tipo_arquivo);
        free(nome_arquivo_indice);
        free(nome_arquivo_entrada);
        break;

    case UPDATE:
        tipo_arquivo = read_word(stdin);
        nome_arquivo_entrada = read_word(stdin);
        nome_arquivo_indice = read_word(stdin);

        int num_updates;
        scanf("%d%*c", &num_updates);

        int *qtt_campos_busca = malloc(num_updates * sizeof(int));
        nome_campos_multireg = malloc(num_updates * sizeof(char **));
        valor_campos_multireg = malloc(num_updates * sizeof(char **));

        int *qtt_campos_updates = malloc(num_updates * sizeof(int));
        char ***nome_campos_update, ***valor_campos_update;
        nome_campos_update = malloc(num_updates * sizeof(char **));
        valor_campos_update = malloc(num_updates * sizeof(char **));

        for (int i = 0; i < num_updates; i++)
        {
            scanf("%d%*c", &qtt_campos_busca[i]);

            nome_campos_multireg[i] = malloc(qtt_campos_busca[i] * sizeof(char *));
            valor_campos_multireg[i] = malloc(qtt_campos_busca[i] * sizeof(char *));

            for (int j = 0; j < qtt_campos_busca[i]; j++)
            {
                nome_campos_multireg[i][j] = read_word(stdin);
                if (nome_campos_multireg[i][j][0] == '\0')
                    break;

                if (strcmp(nome_campos_multireg[i][j], "sigla") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "cidade") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "modelo") == 0 ||
                    strcmp(nome_campos_multireg[i][j], "marca") == 0)
                {
                    valor_campos_multireg[i][j] = read_quotes2(stdin);
                }
                else
                {
                    valor_campos_multireg[i][j] = read_word(stdin);
                }
            }

            scanf("%d%*c", &qtt_campos_updates[i]);

            nome_campos_update[i] = malloc(qtt_campos_updates[i] * sizeof(char *));
            valor_campos_update[i] = malloc(qtt_campos_updates[i] * sizeof(char *));

            for (int j = 0; j < qtt_campos_updates[i]; j++)
            {
                nome_campos_update[i][j] = read_word(stdin);
                if (nome_campos_update[i][j][0] == '\0')
                    break;

                if (strcmp(nome_campos_update[i][j], "sigla") == 0 ||
                    strcmp(nome_campos_update[i][j], "cidade") == 0 ||
                    strcmp(nome_campos_update[i][j], "modelo") == 0 ||
                    strcmp(nome_campos_update[i][j], "marca") == 0)
                {
                    valor_campos_update[i][j] = read_quotes2(stdin);
                }
                else
                {
                    valor_campos_update[i][j] = read_word(stdin);
                }
            }
        }

        findex = open_file(nome_arquivo_indice, "rb", 1);
        fp = open_file(nome_arquivo_entrada, "rb+", 1);

        /* ------ Remove registro arquivo de dados ------ */

        atualiza_status(fp, '0');
        atualiza_status(findex, '0');

        if (isTipo1(tipo_arquivo))
            atualiza_registro_por_parametro_tipo1(fp, findex, nome_campos_multireg, valor_campos_multireg, nome_campos_update, valor_campos_update, qtt_campos_busca, qtt_campos_updates, num_updates);

        if (isTipo2(tipo_arquivo))
            atualiza_registro_por_parametro_tipo2(fp, findex, nome_campos_multireg, valor_campos_multireg, nome_campos_update, valor_campos_update, qtt_campos_busca, qtt_campos_updates, num_updates);

        /* ------ Refaz arquivo de índice ------ */

        if (isTipo1(tipo_arquivo))
            grava_registro_indice_tipo1(fp, findex);
        if (isTipo2(tipo_arquivo))
            grava_registro_indice_tipo2(fp, findex);

        atualiza_status(findex, '1');
        atualiza_status(fp, '1');

        fclose(fp);
        fclose(findex);

        binarioNaTela(nome_arquivo_entrada);
        binarioNaTela(nome_arquivo_indice);

        for (int i = 0; i < num_updates; i++)
        {
            for (int j = 0; j < qtt_campos_busca[i]; j++)
            {
                free(nome_campos_multireg[i][j]);
                free(valor_campos_multireg[i][j]);
            }

            for (int j = 0; j < qtt_campos_updates[i]; j++)
            {
                free(nome_campos_update[i][j]);
                free(valor_campos_update[i][j]);
            }

            free(nome_campos_multireg[i]);
            free(valor_campos_multireg[i]);
            free(nome_campos_update[i]);
            free(valor_campos_update[i]);
        }
        free(nome_campos_multireg);
        free(valor_campos_multireg);
        free(nome_campos_update);
        free(valor_campos_update);

        free(qtt_campos_busca);
        free(qtt_campos_updates);

        free(tipo_arquivo);
        free(nome_arquivo_indice);
        free(nome_arquivo_entrada);

        break;
    }
}