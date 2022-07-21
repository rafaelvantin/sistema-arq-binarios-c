#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/tipo1.h"
#include "headers/tipo2.h"

#include "headers/aux_functions.h"

/*
 * @brief Dado fp, le o registro tipo2 e retorna ele
 */
tipo2_t le_registro_dados_tipo2(FILE *fp)
{
    tipo2_t registro;
    int tamanho;
    char cod, trash;

    fread(&registro.removido, sizeof(char), 1, fp);
    fread(&registro.tamanhoRegistro, sizeof(int), 1, fp);
    fread(&registro.prox, sizeof(long long), 1, fp);

    fread(&registro.id, sizeof(int), 1, fp);
    fread(&registro.ano, sizeof(int), 1, fp);
    fread(&registro.qtt, sizeof(int), 1, fp);
    fread(registro.sigla, sizeof(char), 2, fp);
    registro.sigla[2] = '\0';

    registro.cidade = NULL;
    registro.marca = NULL;
    registro.modelo = NULL;

    int count = 22;

    while (count < registro.tamanhoRegistro)
    {
        fread(&tamanho, sizeof(int), 1, fp);
        fread(&cod, sizeof(char), 1, fp);

        if (cod == '0')
        {
            registro.cidade = malloc(sizeof(char) * (tamanho + 1));
            fread(registro.cidade, sizeof(char), tamanho, fp);
            registro.cidade[tamanho] = '\0';
            registro.tamCidade = tamanho;
            count += tamanho + 5;
        }
        else if (cod == '1')
        {
            registro.marca = malloc(sizeof(char) * (tamanho + 1));
            fread(registro.marca, sizeof(char), tamanho, fp);
            registro.marca[tamanho] = '\0';
            registro.tamMarca = tamanho;
            count += tamanho + 5;
        }
        else if (cod == '2')
        {
            registro.modelo = malloc(sizeof(char) * (tamanho + 1));
            fread(registro.modelo, sizeof(char), tamanho, fp);
            registro.modelo[tamanho] = '\0';
            registro.tamModelo = tamanho;
            count += tamanho + 5;
        }

        if (count < registro.tamanhoRegistro)
        {
            fread(&trash, sizeof(char), 1, fp);
            while (trash == '$')
            {
                count++;
                fread(&trash, sizeof(char), 1, fp);
            }

            fseek(fp, -1, SEEK_CUR);
        }
    }

    return registro;
}

/*
 * @brief Recebe o registro e printa com a formatação adequada
 */
void printa_registro_tipo2(tipo2_t registro)
{
    printf("MARCA DO VEICULO: %s\n", registro.marca == NULL ? "NAO PREENCHIDO" : registro.marca);
    printf("MODELO DO VEICULO: %s\n", registro.modelo == NULL ? "NAO PREENCHIDO" : registro.modelo);
    if (registro.ano != -1)
        printf("ANO DE FABRICACAO: %d\n", registro.ano);
    else
        printf("ANO DE FABRICACAO: NAO PREENCHIDO\n");
    printf("NOME DA CIDADE: %s\n", registro.cidade == NULL ? "NAO PREENCHIDO" : registro.cidade);
    printf("QUANTIDADE DE VEICULOS: %d\n\n", registro.qtt);
}

/*
 * @brief Busca binaria para o tipo1, onde retorna o offset
 */
long long search_byteoffset_by_id(FILE *findex, int neededId)
{
    int min, max;
    int posMin, posMax;

    posMin = 0;

    fseek(findex, -12, SEEK_END);
    posMax = (ftell(findex) - 1) / 12;

    long long byteoffset = -1;
    int lastIteration = -1;
    int thisId;

    // Realiza a busca binaria baseado na posição do registro
    while (posMin < posMax)
    {
        int predict = posMin + ((posMax - posMin) / 2);

        // Calcula o offset no arquivo indice baseado na posição
        int offset = 1 + 12 * predict;

        fseek(findex, offset, SEEK_SET);
        fread(&thisId, sizeof(int), 1, findex);

        if (thisId == neededId)
        {
            fread(&byteoffset, sizeof(long long), 1, findex);
            break;
        }

        if (thisId < neededId)
        {
            posMin = predict + 1;
            lastIteration = 0;
        }
        if (thisId > neededId)
        {
            posMax = predict - 1;
            lastIteration = 1;
        }
    }
    // Ve os casos de borda apos encerrar o loop

    // Ve se o id está no posMax
    if (lastIteration == 0)
    {
        fseek(findex, 1 + posMax * 12, SEEK_SET);
        fread(&thisId, sizeof(int), 1, findex);
        if (thisId == neededId)
            fread(&byteoffset, sizeof(long long), 1, findex);
    }

    // Ve se o id está no posMin
    if (lastIteration == 1)
    {
        fseek(findex, 1 + posMin * 12, SEEK_SET);
        fread(&thisId, sizeof(int), 1, findex);
        if (thisId == neededId)
            fread(&byteoffset, sizeof(long long), 1, findex);
    }

    return byteoffset;
}

/*
 * @brief Checa se os campos do arquivo satisfazem as condicoes passadas do usuario
 */
int check_condicoes_sao_satisfeitas_tipo2(tipo2_t registro, char **nome_campos, char **valor_campos, int num_parametros)
{
    int aux[num_parametros];
    for (int i = 0; i < num_parametros; i++)
        aux[i] = 0;

    for (int i = 0; i < num_parametros; i++)
    {
        if (strcmp(nome_campos[i], "id") == 0 && atoi(valor_campos[i]) == registro.id)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "ano") == 0 && atoi(valor_campos[i]) == registro.ano)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "qtt") == 0 && atoi(valor_campos[i]) == registro.qtt)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "sigla") == 0 && strcmp(valor_campos[i], registro.sigla) == 0)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "cidade") == 0 && registro.cidade != NULL && strcmp(valor_campos[i], registro.cidade) == 0)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "marca") == 0 && registro.marca != NULL && strcmp(valor_campos[i], registro.marca) == 0)
            aux[i] = 1;
        else if (strcmp(nome_campos[i], "modelo") == 0 && registro.modelo != NULL && strcmp(valor_campos[i], registro.modelo) == 0)
            aux[i] = 1;
    }

    int valid = 1;
    for (int i = 0; i < num_parametros; i++)
    {
        if (aux[i] != 1)
        {
            valid = 0;
            break;
        }
    }

    return valid;
}