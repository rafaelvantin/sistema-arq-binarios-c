#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"

#include "../headers/aux_functions.h"

/*
 * @brief Estrutura do cabeçalho para tipo 1
 */
struct cabecalho1
{
    char status;
    int topo;
    char descricao[40];
    char desC1[22];
    char desC2[19];
    char desC3[24];
    char desC4[8];
    char codC5;
    char desC5[16];
    char codC6;
    char desC6[18];
    char codC7;
    char desC7[19];
    int proxRRN;
    int nroRegRem;
};

/*
 * @brief Estrutura do cabeçalho para tipo 2
 */
struct cabecalho2
{
    char status;
    long long topo;
    char descricao[40];
    char desC1[22];
    char desC2[19];
    char desC3[24];
    char desC4[8];
    char codC5;
    char desC5[16];
    char codC6;
    char desC6[18];
    char codC7;
    char desC7[19];
    long long proxByteOffset;
    int nroRegRem;
};

/*
 * @brief Cria uma estrutura para o cabeçalho de tipo 1 e preenche ela com valores default
 */
cabecalho1_t define_cabecalho()
{
    cabecalho1_t novo;

    novo.status = '1';
    novo.topo = -1;
    strcpy(novo.descricao, "LISTAGEM DA FROTA DOS VEICULOS NO BRASIL");
    strcpy(novo.desC1, "CODIGO IDENTIFICADOR: ");
    strcpy(novo.desC2, "ANO DE FABRICACAO: ");
    strcpy(novo.desC3, "QUANTIDADE DE VEICULOS: ");
    strcpy(novo.desC4, "ESTADO: ");
    novo.codC5 = '0';
    strcpy(novo.desC5, "NOME DA CIDADE: ");
    novo.codC6 = '1';
    strcpy(novo.desC6, "MARCA DO VEICULO: ");
    novo.codC7 = '2';
    strcpy(novo.desC7, "MODELO DO VEICULO: ");
    novo.proxRRN = 0;
    novo.nroRegRem = 0;

    return novo;
}

/*
 * @brief Escreve o cabeçalho de tipo 1 no arquivo
 */
void inicializa_cabecalho_tipo1(FILE *fp)
{
    // Cria uma nova estrutura do cabeçálho e atribui a ela os valores default
    cabecalho1_t cabec = define_cabecalho();

    fwrite(&cabec.status, sizeof(char), 1, fp);
    fwrite(&cabec.topo, sizeof(int), 1, fp);
    fwrite(cabec.descricao, sizeof(char), sizeof(cabec.descricao), fp);
    fwrite(cabec.desC1, sizeof(char), sizeof(cabec.desC1), fp);
    fwrite(cabec.desC2, sizeof(char), sizeof(cabec.desC2), fp);
    fwrite(cabec.desC3, sizeof(char), sizeof(cabec.desC3), fp);
    fwrite(cabec.desC4, sizeof(char), sizeof(cabec.desC4), fp);
    fwrite(&cabec.codC5, sizeof(char), 1, fp);
    fwrite(cabec.desC5, sizeof(char), sizeof(cabec.desC5), fp);
    fwrite(&cabec.codC6, sizeof(char), 1, fp);
    fwrite(cabec.desC6, sizeof(char), sizeof(cabec.desC6), fp);
    fwrite(&cabec.codC7, sizeof(char), 1, fp);
    fwrite(cabec.desC7, sizeof(char), sizeof(cabec.desC7), fp);
    fwrite(&cabec.proxRRN, sizeof(int), 1, fp);
    fwrite(&cabec.nroRegRem, sizeof(int), 1, fp);
}

/*
 * @brief Escreve no arquivo um registro de tipo 1
 */
void grava_registro_tipo1(FILE *fp, tipo1_t dados)
{
    // Variável auxiliar para identificar quando há necessidade de preencher com lixo o registro
    int count = 19;

    // Valores default do registro
    char codC5 = '0', codC6 = '1', codC7 = '2';
    dados.removido = '0';
    dados.prox = -1;

    // Início da escrita do registro
    fwrite(&dados.removido, sizeof(char), 1, fp);
    fwrite(&dados.prox, sizeof(int), 1, fp);
    fwrite(&dados.id, sizeof(int), 1, fp);
    fwrite(&dados.ano, sizeof(int), 1, fp);
    fwrite(&dados.qtt, sizeof(int), 1, fp);

    fwrite(dados.sigla, sizeof(char), 2, fp);

    if (dados.tamCidade > 0)
    {
        fwrite(&dados.tamCidade, sizeof(int), 1, fp);
        fwrite(&codC5, sizeof(char), 1, fp);
        fwrite(dados.cidade, sizeof(char), dados.tamCidade, fp);
        count += dados.tamCidade + 5;

        free(dados.cidade);
    }

    if (dados.tamMarca > 0)
    {
        fwrite(&dados.tamMarca, sizeof(int), 1, fp);
        fwrite(&codC6, sizeof(char), 1, fp);
        fwrite(dados.marca, sizeof(char), dados.tamMarca, fp);
        count += dados.tamMarca + 5;

        free(dados.marca);
    }

    if (dados.tamModelo > 0)
    {
        fwrite(&dados.tamModelo, sizeof(int), 1, fp);
        fwrite(&codC7, sizeof(char), 1, fp);
        fwrite(dados.modelo, sizeof(char), dados.tamModelo, fp);
        count += dados.tamModelo + 5;

        free(dados.modelo);
    }

    // Se necessário preenche com lixo o registro
    while (count < REGISTER_SIZE)
    {
        fwrite("$", sizeof(char), 1, fp);
        count++;
    }
}

/*
 * @brief Lê um registro de tipo 1 do arquivo csv e retorna seus dados
 */
tipo1_t le_registro_csv_tipo1(FILE *fp)
{
    tipo1_t novo;

    // Variáveis auxiliares
    char *aux;
    int count_words = 0;

    // --------------- ID ---------------
    aux = read_till_delimitor(fp);
    novo.id = atoi(aux);
    free(aux);

    // --------------- ANO ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        novo.ano = atoi(aux);
    else
        novo.ano = -1;
    free(aux);

    // --------------- CIDADE ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.cidade = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.cidade, aux);
        novo.tamCidade = count_words;
    }
    else
        novo.tamCidade = 0;
    free(aux);
    count_words = 0;

    // --------------- QUANTIDADE ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        novo.qtt = atoi(aux);
    else
        novo.qtt = -1;
    free(aux);

    // --------------- SIGLA ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        strcpy(novo.sigla, aux);
    else
        strcpy(novo.sigla, "$$");
    free(aux);

    // --------------- MARCA ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.marca = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.marca, aux);
        novo.tamMarca = count_words;
    }
    else
    {
        novo.tamMarca = 0;
    }
    free(aux);
    count_words = 0;

    // --------------- MODELO ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.modelo = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.modelo, aux);
        novo.tamModelo = count_words;
    }
    else
        novo.tamModelo = 0;
    free(aux);
    count_words;

    return novo;
}

/*
 * @brief Cria uma estrutura para o cabeçalho de tipo 2 e preenche ela com valores default
 */
cabecalho2_t define_cabecalho2()
{
    cabecalho2_t novo;
    novo.status = '1';
    novo.topo = -1;
    strcpy(novo.descricao, "LISTAGEM DA FROTA DOS VEICULOS NO BRASIL");
    strcpy(novo.desC1, "CODIGO IDENTIFICADOR: ");
    strcpy(novo.desC2, "ANO DE FABRICACAO: ");
    strcpy(novo.desC3, "QUANTIDADE DE VEICULOS: ");
    strcpy(novo.desC4, "ESTADO: ");
    novo.codC5 = '0';
    strcpy(novo.desC5, "NOME DA CIDADE: ");
    novo.codC6 = '1';
    strcpy(novo.desC6, "MARCA DO VEICULO: ");
    novo.codC7 = '2';
    strcpy(novo.desC7, "MODELO DO VEICULO: ");
    novo.proxByteOffset = END_HEADER2_OFFSET;
    novo.nroRegRem = 0;
    return novo;
}

/*
 * @brief Escreve o cabeçalho de tipo 2 no arquivo
 */
void inicializa_cabecalho_tipo2(FILE *fp)
{
    // Cria uma nova estrutura do cabeçálho e atribui a ela os valores default
    cabecalho2_t cabec = define_cabecalho2();

    fwrite(&cabec.status, sizeof(char), 1, fp);
    fwrite(&cabec.topo, sizeof(long long), 1, fp);
    fwrite(cabec.descricao, sizeof(char), 40, fp);
    fwrite(cabec.desC1, sizeof(char), 22, fp);
    fwrite(cabec.desC2, sizeof(char), 19, fp);
    fwrite(cabec.desC3, sizeof(char), 24, fp);
    fwrite(cabec.desC4, sizeof(char), 8, fp);
    fwrite(&cabec.codC5, sizeof(char), 1, fp);
    fwrite(cabec.desC5, sizeof(char), 16, fp);
    fwrite(&cabec.codC6, sizeof(char), 1, fp);
    fwrite(cabec.desC6, sizeof(char), 18, fp);
    fwrite(&cabec.codC7, sizeof(char), 1, fp);
    fwrite(cabec.desC7, sizeof(char), 19, fp);
    fwrite(&cabec.proxByteOffset, sizeof(long long), 1, fp);
    fwrite(&cabec.nroRegRem, sizeof(int), 1, fp);
}

/*
 * @brief Lê um registro de tipo 2 do arquivo csv e retorna seus dados
 */
tipo2_t le_registro_csv_tipo2(FILE *fp)
{
    tipo2_t novo;
    char *aux;
    int count_words = 0;
    novo.tamanhoRegistro = 22;

    // --------------- ID ---------------
    aux = read_till_delimitor(fp);
    novo.id = atoi(aux);
    free(aux);

    // --------------- ANO ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        novo.ano = atoi(aux);
    else
        novo.ano = -1;
    free(aux);

    // --------------- CIDADE ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.cidade = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.cidade, aux);
        novo.tamCidade = count_words;
        novo.tamanhoRegistro += count_words + 5;
    }
    else
        novo.tamCidade = 0;
    free(aux);
    count_words = 0;

    // --------------- QUANTIDADE ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        novo.qtt = atoi(aux);
    else
        novo.qtt = -1;
    free(aux);

    // --------------- SIGLA ---------------
    aux = read_till_delimitor(fp);
    if (aux[0] != '\0')
        strcpy(novo.sigla, aux);
    else
        strcpy(novo.sigla, "$$");
    free(aux);

    // --------------- MARCA ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.marca = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.marca, aux);
        novo.tamMarca = count_words;
        novo.tamanhoRegistro += count_words + 5;
    }
    else
    {
        novo.tamMarca = 0;
    }
    free(aux);
    count_words = 0;

    // --------------- MODELO ---------------
    aux = read_and_count_till_delimitor(fp, &count_words);
    if (aux[0] != '\0')
    {
        novo.modelo = (char *)malloc(sizeof(char) * (count_words + 1));
        strcpy(novo.modelo, aux);
        novo.tamModelo = count_words;
        novo.tamanhoRegistro += count_words + 5;
    }
    else
        novo.tamModelo = 0;
    free(aux);
    count_words;

    return novo;
}

/*
 * @brief Escreve no arquivo um registro de tipo 2
 */
void grava_registro_tipo2(FILE *fp, tipo2_t dados)
{
    // Valores default do registro
    char codC5 = '0', codC6 = '1', codC7 = '2';
    dados.removido = '0';
    dados.prox = -1;

    // Início da escrita do registro
    fwrite(&dados.removido, sizeof(char), 1, fp);
    fwrite(&dados.tamanhoRegistro, sizeof(int), 1, fp);
    fwrite(&dados.prox, sizeof(long long), 1, fp);
    fwrite(&dados.id, sizeof(int), 1, fp);
    fwrite(&dados.ano, sizeof(int), 1, fp);
    fwrite(&dados.qtt, sizeof(int), 1, fp);
    fwrite(dados.sigla, sizeof(char), 2, fp);

    if (dados.tamCidade > 0)
    {
        fwrite(&dados.tamCidade, sizeof(int), 1, fp);
        fwrite(&codC5, sizeof(char), 1, fp);
        fwrite(dados.cidade, sizeof(char), dados.tamCidade, fp);

        free(dados.cidade);
    }

    if (dados.tamMarca > 0)
    {
        fwrite(&dados.tamMarca, sizeof(int), 1, fp);
        fwrite(&codC6, sizeof(char), 1, fp);
        fwrite(dados.marca, sizeof(char), dados.tamMarca, fp);

        free(dados.marca);
    }

    if (dados.tamModelo > 0)
    {
        fwrite(&dados.tamModelo, sizeof(int), 1, fp);
        fwrite(&codC7, sizeof(char), 1, fp);
        fwrite(dados.modelo, sizeof(char), dados.tamModelo, fp);

        free(dados.modelo);
    }
}