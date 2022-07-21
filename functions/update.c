#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"

#include "../headers/aux_functions.h"
#include "../headers/logic_delete.h"
#include "../headers/create_bin_file.h"
#include "../headers/insert_into.h"

/*
 * @brief Conta o tamanho de uma string ignorando o '\0' e retorna o valor encontrado
 */
int get_tamanho_string(char *string)
{
    int count = 0;
    while (string[count] != '\0')
    {
        count++;
    }

    return count;
}

/*
* @brief Dado o registro lido e os parâmetros de alteração passados na função atualiza_registro_por_parametro_tipo2 cria uma struct tipo2_t
         já atualizada com os valores_update.
*/
tipo2_t update_registro_tipo2(tipo2_t registro, char **nome_campos_update, char **valor_campos_update, int qtt_campos_updates)
{
    // Verifica todos os campos
    for (int i = 0; i < qtt_campos_updates; i++)
    {
        // Altera ID
        if (strcmp(nome_campos_update[i], "id") == 0)
        {
            registro.id = atoi(valor_campos_update[i]);
        }

        // Altera ano
        if (strcmp(nome_campos_update[i], "ano") == 0)
        {
            registro.ano = atoi(valor_campos_update[i]);
        }

        // Altera qtt
        if (strcmp(nome_campos_update[i], "qtt") == 0)
        {
            registro.qtt = atoi(valor_campos_update[i]);
        }

        // Altera sigla
        if (strcmp(nome_campos_update[i], "sigla") == 0)
        {
            strcpy(registro.sigla, valor_campos_update[i]);
        }

        // Altera cidade e tamanho cidade
        if (strcmp(nome_campos_update[i], "cidade") == 0)
        {
            // Caso o campo cidade seja atualizado, libera da memória o valor antigo
            if (registro.cidade != NULL)
            {
                free(registro.cidade);
                registro.cidade = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamCidade = get_tamanho_string(valor_campos_update[i]);
                registro.cidade = malloc(sizeof(char) * tamCidade + 1);
                strcpy(registro.cidade, valor_campos_update[i]);
                registro.tamCidade = tamCidade;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.cidade = NULL;
                registro.tamCidade = 0;
            }
        }

        // Se cidade não for um parâmetro e o registro original também não tiver um campo cidade, define o tamCidade = 0
        if (registro.cidade == NULL)
        {
            registro.tamCidade = 0;
        }

        // Altera marca e tamMarca
        if (strcmp(nome_campos_update[i], "marca") == 0)
        {
            // Caso o campo marca seja atualizado, libera da memória o valor antigo
            if (registro.marca != NULL)
            {
                free(registro.marca);
                registro.marca = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamMarca = get_tamanho_string(valor_campos_update[i]);
                registro.marca = malloc(sizeof(char) * tamMarca + 1);
                strcpy(registro.marca, valor_campos_update[i]);
                registro.tamMarca = tamMarca;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.marca = NULL;
                registro.tamMarca = 0;
            }
        }

        // Se marca não for um parâmetro e o registro original também não tiver um campo marca, define o tamMarca = 0
        if (registro.marca == NULL)
        {
            registro.tamMarca = 0;
        }

        // Altera modelo e tamModelo
        if (strcmp(nome_campos_update[i], "modelo") == 0)
        {
            // Caso o campo modelo seja atualizado, libera da memória o valor antigo
            if (registro.modelo != NULL)
            {
                free(registro.modelo);
                registro.modelo = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamModelo = get_tamanho_string(valor_campos_update[i]);
                registro.modelo = malloc(sizeof(char) * tamModelo + 1);
                strcpy(registro.modelo, valor_campos_update[i]);
                registro.tamModelo = tamModelo;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.modelo = NULL;
                registro.tamModelo = 0;
            }
        }

        // Se modelo não for um parâmetro e o registro original também não tiver um campo modelo, define o tamModelo = 0
        if (registro.modelo == NULL)
        {
            registro.tamModelo = 0;
        }
    }

    // Calcula o tamanho do novo registro
    registro.tamanhoRegistro = 22;

    if (registro.tamCidade > 0)
        registro.tamanhoRegistro += 5 + registro.tamCidade;

    if (registro.tamMarca > 0)
        registro.tamanhoRegistro += 5 + registro.tamMarca;

    if (registro.tamModelo > 0)
        registro.tamanhoRegistro += 5 + registro.tamModelo;

    return registro;
}

/*
* @brief Dado o registro lido e os parâmetros de alteração passados na função atualiza_registro_por_parametro_tipo1 cria uma struct tipo1_t
         já atualizada com os valores_update.
*/
tipo1_t update_registro_tipo1(tipo1_t registro, char **nome_campos_update, char **valor_campos_update, int qtt_campos_updates)
{
    // Verifica todos os campos
    for (int i = 0; i < qtt_campos_updates; i++)
    {
        // Altera ID
        if (strcmp(nome_campos_update[i], "id") == 0)
        {
            registro.id = atoi(valor_campos_update[i]);
        }

        // Altera ano
        if (strcmp(nome_campos_update[i], "ano") == 0)
        {
            registro.ano = atoi(valor_campos_update[i]);
        }

        // Altera qtt
        if (strcmp(nome_campos_update[i], "qtt") == 0)
        {
            registro.qtt = atoi(valor_campos_update[i]);
        }

        // Altera sigla
        if (strcmp(nome_campos_update[i], "sigla") == 0)
        {
            strcpy(registro.sigla, valor_campos_update[i]);
        }

        // Altera cidade e tamanho cidade
        if (strcmp(nome_campos_update[i], "cidade") == 0)
        {
            // Caso o campo cidade seja atualizado, libera da memória o valor antigo
            if (registro.cidade != NULL)
            {
                free(registro.cidade);
                registro.cidade = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamCidade = get_tamanho_string(valor_campos_update[i]);
                registro.cidade = malloc(sizeof(char) * tamCidade + 1);
                strcpy(registro.cidade, valor_campos_update[i]);
                registro.tamCidade = tamCidade;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.cidade = NULL;
                registro.tamCidade = 0;
            }
        }

        // Se cidade não for um parâmetro e o registro original também não tiver um campo cidade, define o tamCidade = 0
        if (registro.cidade == NULL)
        {
            registro.tamCidade = 0;
        }

        // Altera marca e tamMarca
        if (strcmp(nome_campos_update[i], "marca") == 0)
        {
            // Caso o campo marca seja atualizado, libera da memória o valor antigo
            if (registro.marca != NULL)
            {
                free(registro.marca);
                registro.marca = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamMarca = get_tamanho_string(valor_campos_update[i]);
                registro.marca = malloc(sizeof(char) * tamMarca + 1);
                strcpy(registro.marca, valor_campos_update[i]);
                registro.tamMarca = tamMarca;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.marca = NULL;
                registro.tamMarca = 0;
            }
        }

        // Se marca não for um parâmetro e o registro original também não tiver um campo marca, define o tamMarca = 0
        if (registro.marca == NULL)
        {
            registro.tamMarca = 0;
        }

        // Altera modelo e tamModelo
        if (strcmp(nome_campos_update[i], "modelo") == 0)
        {
            // Caso o campo modelo seja atualizado, libera da memória o valor antigo
            if (registro.modelo != NULL)
            {
                free(registro.modelo);
                registro.modelo = NULL;
            }

            // Caso a mudança seja para algum novo valor
            if (strcmp(valor_campos_update[i], "NULO") != 0)
            {
                int tamModelo = get_tamanho_string(valor_campos_update[i]);
                registro.modelo = malloc(sizeof(char) * tamModelo + 1);
                strcpy(registro.modelo, valor_campos_update[i]);
                registro.tamModelo = tamModelo;
            }

            // Caso a mudança seja para nulo
            else
            {
                registro.modelo = NULL;
                registro.tamModelo = 0;
            }
        }

        // Se modelo não for um parâmetro e o registro original também não tiver um campo modelo, define o tamModelo = 0
        if (registro.modelo == NULL)
        {
            registro.tamModelo = 0;
        }
    }

    return registro;
}

/*
* @brief Realiza N alterações de dados do registro, dada a quantidade de campos que devem ser buscados, seus nomes e valores, e dada também a
         quantidade de campos a serem atualizados, seus nomes e valores. Realiza busca no arquivo de índices caso o campo de busca seja o ID.
*/
void atualiza_registro_por_parametro_tipo1(FILE *fp, FILE *findex, char ***nome_campos_procura, char ***valor_campos_procura, char ***nome_campos_update,
                                           char ***valor_campos_update, int *qtt_campos_busca, int *qtt_campos_updates, int num_updates)
{
    for (int i = 0; i < num_updates; i++)
    {
        // Struct onde serão salvos sos registros lidos
        tipo1_t registro;

        // Variáveis auxiliares para a leitura dos registros
        int tamanho;
        char cod, trash;

        // Search offset by id
        int rrn = -1;

        for (int j = 0; j < qtt_campos_busca[i]; j++)
        {
            if (strcmp(nome_campos_procura[i][j], "id") == 0)
                rrn = search_RRN_by_id(findex, atoi(valor_campos_procura[i][j]));
        }

        if (rrn != -1)
            fseek(fp, END_HEADER1_OFFSET + (rrn * REGISTER_SIZE), SEEK_SET);
        else
            fseek(fp, END_HEADER1_OFFSET, SEEK_SET);

        // Realiza leitura até o fim do arquivo
        while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
        {
            // Ignora registros logicamente removidos e passa para o próximo
            if (registro.removido == '1')
            {
                fseek(fp, REGISTER_SIZE - 1, SEEK_CUR);
                continue;
            }

            fseek(fp, -1, SEEK_CUR);

            long offset_atualizado_reg = ftell(fp);

            registro = le_registro_dados_tipo1(fp);

            int update = check_condicoes_sao_satisfeitas_tipo1(registro, nome_campos_procura[i], valor_campos_procura[i], qtt_campos_busca[i]);

            if (update)
            {
                // Cria um novo registro atualizado já feitas as substituições
                tipo1_t novo_registro = update_registro_tipo1(registro, nome_campos_update[i], valor_campos_update[i], qtt_campos_updates[i]);

                // Salva a posição atual do arquivo
                long offset_antes_atualizacao = ftell(fp);

                // Volta para o início do registro
                fseek(fp, offset_atualizado_reg, SEEK_SET);

                // Grava o novo_registro
                grava_registro_tipo1(fp, novo_registro);

                // Volta para o posição do arquivo antes da atualização
                fseek(fp, offset_antes_atualizacao, SEEK_SET);
            }

            // Read trash and free's

            while ((ftell(fp) - END_HEADER1_OFFSET) % 97 > 1)
            {
                fread(&trash, sizeof(char), 1, fp);
            }

            if (!update)
            {
                if (registro.cidade != NULL)
                    free(registro.cidade);
                if (registro.marca != NULL)
                    free(registro.marca);
                if (registro.modelo != NULL)
                    free(registro.modelo);
            }

            if (rrn != -1)
                break;
        }
    }
}

/*
* @brief Realiza N alterações de dados do registro, dada a quantidade de campos que devem ser buscados, seus nomes e valores, e dada também a
         quantidade de campos a serem atualizados, seus nomes e valores. Realiza busca no arquivo de índices caso o campo de busca seja o ID.
*/
void atualiza_registro_por_parametro_tipo2(FILE *fp, FILE *findex, char ***nome_campos_procura, char ***valor_campos_procura, char ***nome_campos_update,
                                           char ***valor_campos_update, int *qtt_campos_busca, int *qtt_campos_updates, int num_updates)
{
    for (int i = 0; i < num_updates; i++)
    {
        // Struct onde serão salvos sos registros lidos
        tipo2_t registro;

        // Variáveis auxiliares para a leitura dos registros
        int tamanho;
        char cod;

        // Search offset by id
        long long byteoffset = -1;

        for (int j = 0; j < qtt_campos_busca[i]; j++)
        {
            if (strcmp(nome_campos_procura[i][j], "id") == 0)
                byteoffset = search_byteoffset_by_id(findex, atoi(valor_campos_procura[i][j]));
        }

        if (byteoffset != -1)
            fseek(fp, byteoffset, SEEK_SET);
        else
            fseek(fp, END_HEADER2_OFFSET, SEEK_SET);

        // Realiza leitura até o fim do arquivo
        while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
        {
            fread(&registro.tamanhoRegistro, sizeof(int), 1, fp);

            // Ignora registros logicamente removidos e passa para o próximo
            if (registro.removido == '1')
            {
                fseek(fp, registro.tamanhoRegistro, SEEK_CUR);
                continue;
            }

            // Volta para o começo do registro
            fseek(fp, -5, SEEK_CUR);

            long long offset_atualizado_reg = ftell(fp);

            registro = le_registro_dados_tipo2(fp);

            int update = check_condicoes_sao_satisfeitas_tipo2(registro, nome_campos_procura[i], valor_campos_procura[i], qtt_campos_busca[i]);

            // Realiza a atualização do registro
            if (update)
            {
                // Salva o tamanho original do registro
                int tamanho_registro_original = registro.tamanhoRegistro;

                // Cria um novo registro atualizado já feitas as substituições
                tipo2_t novo_registro = update_registro_tipo2(registro, nome_campos_update[i], valor_campos_update[i], qtt_campos_updates[i]);

                // Caso o tamanho do novo registro atualizado seja menos ou igual ao do registro original
                if (novo_registro.tamanhoRegistro <= tamanho_registro_original)
                {
                    // Salva a posição atual do arquivo
                    long offset_antes_atualizacao = ftell(fp);

                    // Volta para o início do arquivo
                    fseek(fp, offset_atualizado_reg, SEEK_SET);

                    // Define o tamanho do novo_registro atualizado como o tamanho do registro original
                    novo_registro.tamanhoRegistro = tamanho_registro_original;

                    // Preenche todo o espaço do registro com lixo ("$")
                    for (int i = 0; i < tamanho_registro_original + 5; i++)
                    {
                        fwrite("$", sizeof(char), 1, fp);
                    }

                    // Volta para o início do registro
                    fseek(fp, offset_atualizado_reg, SEEK_SET);

                    // Grava o novo_registro
                    grava_registro_tipo2(fp, novo_registro);

                    // Volta para o posição do arquivo antes da atualização
                    fseek(fp, offset_antes_atualizacao, SEEK_SET);
                }

                // Caso o tamanho do novo_registro seja maior do que o tamanho do registro original
                else
                {
                    // Salva a posição atual do arquivo
                    long offset_antes_atualizacao = ftell(fp);

                    // Chama a função de remoção para o registro lido
                    remove_registro_tipo2(fp, offset_atualizado_reg);

                    // Chama a função de inserção para o novo_registro
                    insert_into_tipo_2(fp, novo_registro);

                    // Volta para a posição do arquivo antes da atualização
                    fseek(fp, offset_antes_atualizacao, SEEK_SET);
                }
            }

            // Frees
            if (!update)
            {
                if (registro.cidade != NULL)
                    free(registro.cidade);
                if (registro.marca != NULL)
                    free(registro.marca);
                if (registro.modelo != NULL)
                    free(registro.modelo);
            }

            if (byteoffset != -1)
                break;
        }
    }
}