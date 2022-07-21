#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/tipo1.h"
#include "headers/tipo2.h"
#include "headers/indice.h"

#include "headers/aux_functions.h"

typedef struct key key_tipo1_t;
typedef struct key2 key_tipo2_t;

typedef struct no_tipo1 no_tipo1_t;
typedef struct no_tipo2 no_tipo2_t;

typedef struct working_no_tipo1 working_no_tipo1_t;
typedef struct working_no_tipo2 working_no_tipo2_t;

/*
 * @brief Estrutura das chaves do registro para tipo 1 e tipo 2 respectivamente
 */

struct key
{
    int id;
    int rrn;
};

struct key2
{
    int id;
    long long byteOffset;
};

/*
 * @brief Estrutura dos nó (pages) do registro para tipo 1 e tipo 2 respectivamente
 */

struct no_tipo1
{
    char tipoNo;
    int nroChaves;
    key_tipo1_t keys[3];
    int filhos[4];
};

struct no_tipo2
{
    char tipoNo;
    int nroChaves;
    key_tipo2_t keys[3];
    int filhos[4];
};

/*
 * @brief Estrutura dos nó (pages) de trabalho usados no split de tipo 1 e tipo 2 respectivamente
 */

struct working_no_tipo1
{
    int nroChaves;
    key_tipo1_t keys[4];
    int filhos[5];
};

struct working_no_tipo2
{
    int nroChaves;
    key_tipo2_t keys[4];
    int filhos[5];
};

/*
 * @brief Função get() para retornar o próximo ID de um arquivo de tipo 1 e tipo 2 respectivamente
 */

int get_prox_id_tipo1(FILE *fp)
{
    char removido;
    int id;

    fread(&removido, sizeof(char), 1, fp);

    // Ignora registros logicamente removidos
    if (removido == '1')
    {
        fseek(fp, 96L, SEEK_CUR);
        return -1;
    }
    fseek(fp, 4L, SEEK_CUR);
    fread(&id, sizeof(int), 1, fp);

    fseek(fp, 88L, SEEK_CUR);

    return id;
}

int get_prox_id_tipo2(FILE *fp, long long *byteoffset)
{
    *byteoffset = ftell(fp);
    char removido;
    int tamanho_registro;
    int id;

    fread(&removido, sizeof(char), 1, fp);
    fread(&tamanho_registro, sizeof(int), 1, fp);

    fseek(fp, 8L, SEEK_CUR);
    fread(&id, sizeof(int), 1, fp);

    fseek(fp, tamanho_registro - 12, SEEK_CUR);

    // Ignora registros logicamente removidos
    if (removido == '1')
        return -1;

    return id;
}

/*
 * @brief Função que grava no arquivo de índices em árvore-B um novo registro (tipo 1)
 */

void grava_registro_indice_tipo1(FILE *fp, FILE *findex)
{
    int RRN = 0;

    fseek(fp, END_HEADER1_OFFSET, SEEK_SET);
    fseek(findex, END_HEADER_INDEX_OFFSET, SEEK_SET);

    while (!is_eof(fp))
    {
        int id = get_prox_id_tipo1(fp);

        if (id != -1)
        {
            fwrite(&id, sizeof(int), 1, findex);
            fwrite(&RRN, sizeof(int), 1, findex);
        }
        RRN++;
    }
}

/*
 * @brief Função que grava no arquivo de índices em árvore-B um novo registro (tipo 2)
 */

void grava_registro_indice_tipo2(FILE *fp, FILE *findex)
{
    long long byteoffset;

    fseek(fp, END_HEADER2_OFFSET, SEEK_SET);
    fseek(findex, END_HEADER_INDEX_OFFSET, SEEK_SET);

    while (!is_eof(fp))
    {
        int id = get_prox_id_tipo2(fp, &byteoffset);

        if (id != -1)
        {
            fwrite(&id, sizeof(int), 1, findex);
            fwrite(&byteoffset, sizeof(long), 1, findex);
        }
    }
}

/*
 * @brief Função que define o cabeçalho do arquivo de índices em árvore-B
 */

void define_cabecalho_arvb(FILE *findex, char *tipo_arquivo)
{
    fseek(findex, 1, SEEK_SET);
    int noRaiz = -1;
    int proxRRN = 0;
    int nroNos = 0;

    char lixo = '$';

    fwrite(&noRaiz, sizeof(int), 1, findex);
    fwrite(&proxRRN, sizeof(int), 1, findex);
    fwrite(&nroNos, sizeof(int), 1, findex);

    // Dependendo do tipo do arquivo, a quantidade de lixo a ser inserido no fim varia, sendo 32 para tipo 1 e 44 para tipo 2.
    int num_lixos = isTipo1(tipo_arquivo) ? 32 : 44;

    for (int i = 0; i < num_lixos; i++)
        fwrite(&lixo, sizeof(char), 1, findex);
}

/*
 * @brief Função cria um novo nó (page) zerada para árvore-B de tipo 1 e o retorna
 */

no_tipo1_t new_no_arvb_tipo1()
{
    no_tipo1_t no;

    no.tipoNo = '2';
    no.nroChaves = 0;
    for (int i = 0; i < 3; i++)
    {
        no.keys[i].id = -1;
        no.keys[i].rrn = -1;
    }

    for (int i = 0; i < 4; i++)
        no.filhos[i] = -1;

    return no;
}

/*
 * @brief Função cria um novo nó (page) zerada para árvore-B de tipo 2 e o retorna
 */

no_tipo2_t new_no_arvb_tipo2()
{
    no_tipo2_t no;

    no.tipoNo = '2';
    no.nroChaves = 0;
    for (int i = 0; i < 3; i++)
    {
        no.keys[i].id = -1;
        no.keys[i].byteOffset = -1;
    }

    for (int i = 0; i < 4; i++)
        no.filhos[i] = -1;

    return no;
}

/*
 * @brief Função cria um novo nó (page) zerada para o split de tipo 1 e o retorna
 */

working_no_tipo1_t new_working_no_arvb_tipo1()
{
    working_no_tipo1_t w_no;

    w_no.nroChaves = 0;
    for (int i = 0; i < 4; i++)
    {
        w_no.keys[i].id = -1;
        w_no.keys[i].rrn = -1;
    }

    for (int i = 0; i < 5; i++)
        w_no.filhos[i] = -1;

    return w_no;
}

/*
 * @brief Função cria um novo nó (page) zerada para o split de tipo 2 e o retorna
 */

working_no_tipo2_t new_working_no_arvb_tipo2()
{
    working_no_tipo2_t w_no;

    w_no.nroChaves = 0;
    for (int i = 0; i < 4; i++)
    {
        w_no.keys[i].id = -1;
        w_no.keys[i].byteOffset = -1;
    }

    for (int i = 0; i < 5; i++)
        w_no.filhos[i] = -1;

    return w_no;
}

/*
 * @brief Função que lê o próximo registro da árvore de tipo 1 e retorna os valores como uma estrutura de nó.
 */

no_tipo1_t read_no_tipo1(FILE *findex, int current_rrn)
{
    no_tipo1_t no;

    fseek(findex, END_HEADER_ARVB_TIPO1_OFFSET + (current_rrn * NO_SIZE_ARVB_TIPO1), SEEK_SET);

    fread(&no.tipoNo, sizeof(char), 1, findex);
    fread(&no.nroChaves, sizeof(int), 1, findex);

    for (int i = 0; i < 3; i++)
    {
        fread(&no.keys[i].id, sizeof(int), 1, findex);
        fread(&no.keys[i].rrn, sizeof(int), 1, findex);
    }

    for (int i = 0; i < 4; i++)
        fread(&no.filhos[i], sizeof(int), 1, findex);

    return no;
}

/*
 * @brief Função que lê o próximo registro da árvore de tipo 2 e retorna os valores como uma estrutura de nó.
 */

no_tipo2_t read_no_tipo2(FILE *findex, int current_byteOffset)
{
    no_tipo2_t no;

    fseek(findex, END_HEADER_ARVB_TIPO2_OFFSET + (current_byteOffset * NO_SIZE_ARVB_TIPO2), SEEK_SET);

    fread(&no.tipoNo, sizeof(char), 1, findex);
    fread(&no.nroChaves, sizeof(int), 1, findex);

    for (int i = 0; i < 3; i++)
    {
        fread(&no.keys[i].id, sizeof(int), 1, findex);
        fread(&no.keys[i].byteOffset, sizeof(long), 1, findex);
    }

    for (int i = 0; i < 4; i++)
        fread(&no.filhos[i], sizeof(int), 1, findex);

    return no;
}

/*
 * @brief Função que dado um nó de árvore-B tipo 1, grava essa nova página no arquivo índice.
 */

void write_no_arvb_tipo1(FILE *findex, no_tipo1_t no, int rrn)
{
    fseek(findex, END_HEADER_ARVB_TIPO1_OFFSET + (rrn * NO_SIZE_ARVB_TIPO1), SEEK_SET);

    fwrite(&no.tipoNo, sizeof(char), 1, findex);
    fwrite(&no.nroChaves, sizeof(int), 1, findex);

    for (int i = 0; i < 3; i++)
    {
        fwrite(&no.keys[i].id, sizeof(int), 1, findex);
        fwrite(&no.keys[i].rrn, sizeof(int), 1, findex);
    }

    for(int i = 0; i < 4; i++)
        fwrite(&no.filhos[i], sizeof(int), 1, findex);
}

void write_no_arvb_tipo2(FILE *findex, no_tipo2_t no, int rrn)
{
    fseek(findex, END_HEADER_ARVB_TIPO2_OFFSET + (rrn * NO_SIZE_ARVB_TIPO2), SEEK_SET);

    fwrite(&no.tipoNo, sizeof(char), 1, findex);
    fwrite(&no.nroChaves, sizeof(int), 1, findex);

    for (int i = 0; i < 3; i++)
    {
        fwrite(&no.keys[i].id, sizeof(int), 1, findex);
        fwrite(&no.keys[i].byteOffset, sizeof(long long), 1, findex);
    }

    for(int i = 0; i < 4; i++)
        fwrite(&no.filhos[i], sizeof(int), 1, findex);
}

/*
 * @brief Função que realiza o processo de split na árvore-B de tipo 1.
 */

void split_no_arvb_tipo1(FILE *findex, key_tipo1_t *promo_b_key, int *promo_b_rrn, no_tipo1_t *current, key_tipo1_t *promo_key, int *promo_r_child, no_tipo1_t *new_no)
{
    // Cria working page
    working_no_tipo1_t w_no = new_working_no_arvb_tipo1();
    
    // Acha pos onde a nova chave vai ser inserida
    int pos;
    int index_new_key;
    
    for(pos = 0; pos < current->nroChaves; pos++)
    {        
        if(current->keys[pos].id == -1 || promo_b_key->id < current->keys[pos].id)
            break;
    }
    index_new_key = pos;

    // Coloca nova chave na working page
    w_no.keys[pos] = *promo_b_key;
    w_no.filhos[pos + 1] = *promo_b_rrn; 
    w_no.nroChaves++;

    // Adiciona as chaves ja existentes na wp, pulando a pos da chave nova
    int pos_wp = 0;
    for(pos = 0; pos < current->nroChaves; pos++)
    {
        if(pos == index_new_key)
            pos_wp++;

        w_no.keys[pos_wp] = current->keys[pos];
        w_no.nroChaves++;
        pos_wp++;
    }

    // Faz o mesmo das chaves com os filhos
    pos_wp = 0;
    for(pos = 0; pos < current->nroChaves + 1; pos++)
    {
        if(pos == index_new_key + 1)
            pos_wp++;

        w_no.filhos[pos] = current->filhos[pos];
        pos_wp++;
    }

    // Atualiza o parametro dizendo qual a chave que vai ser promovida
    *promo_key = w_no.keys[2];
    *promo_r_child = getProxRRN_arvb(findex);

    // Copia conteudo da WP de volta para Page
    *current = new_no_arvb_tipo1();

    current->keys[0] = w_no.keys[0];
    current->keys[1] = w_no.keys[1];

    current->filhos[0] = w_no.filhos[0];
    current->filhos[1] = w_no.filhos[1];
    current->filhos[2] = w_no.filhos[2];

    current->nroChaves = 2;

    // Copia conteudo de WP para nova pagina irmã
    *new_no = new_no_arvb_tipo1();

    new_no->keys[0] = w_no.keys[3];

    new_no->filhos[0] = w_no.filhos[3];
    new_no->filhos[1] = w_no.filhos[4];

    new_no->nroChaves = 1;
}

/*
 * @brief Função que realiza o processo de split na árvore-B de tipo 2.
 */

void split_no_arvb_tipo2(FILE *findex, key_tipo2_t *promo_b_key, int *promo_b_rrn, no_tipo2_t *current, key_tipo2_t *promo_key, int *promo_r_child, no_tipo2_t *new_no)
{
    // Cria working page
    working_no_tipo2_t w_no = new_working_no_arvb_tipo2();

    // Acha pos onde a nova chave vai ser inserida
    int pos;
    int index_new_key;

    for (pos = 0; pos < current->nroChaves; pos++)
    {
        if (current->keys[pos].id == -1 || promo_b_key->id < current->keys[pos].id)
            break;
    }
    index_new_key = pos;

    // Coloca nova chave na working page
    w_no.keys[pos] = *promo_b_key;
    w_no.filhos[pos + 1] = *promo_b_rrn;
    w_no.nroChaves++;

    // Adiciona as chaves ja existentes na wp, pulando a pos da chave nova
    int pos_wp = 0;
    for (pos = 0; pos < current->nroChaves; pos++)
    {
        if (pos == index_new_key)
            pos_wp++;

        w_no.keys[pos_wp] = current->keys[pos];
        w_no.nroChaves++;
        pos_wp++;
    }


    // Faz o mesmo das chaves com os filhos
    pos_wp = 0;
    for (pos = 0; pos < current->nroChaves + 1; pos++)
    {
        if (pos == index_new_key + 1)
            pos_wp++;

        w_no.filhos[pos] = current->filhos[pos];
        pos_wp++;
    }

    // Atualiza o parametro dizendo qual a chave que vai ser promovida
    *promo_key = w_no.keys[2];
    *promo_r_child = getProxRRN_arvb(findex);

    // Copia conteudo da WP de volta para Page
    *current = new_no_arvb_tipo2();

    current->keys[0] = w_no.keys[0];
    current->keys[1] = w_no.keys[1];

    current->filhos[0] = w_no.filhos[0];
    current->filhos[1] = w_no.filhos[1];
    current->filhos[2] = w_no.filhos[2];

    current->nroChaves = 2;

    // Copia conteudo de WP para nova pagina irmã
    *new_no = new_no_arvb_tipo2();

    new_no->keys[0] = w_no.keys[3];

    new_no->filhos[0] = w_no.filhos[3];
    new_no->filhos[1] = w_no.filhos[4];

    new_no->nroChaves = 1;
}

/*
 * @brief Função que realiza a inserção de um novo registro no arquivo de índices em árvore-B tipo 1.
 */

int insert_arvb_tipo1(FILE *findex, int current_rrn, key_tipo1_t key, key_tipo1_t *promo_key, int *promo_r_child)
{
    // RRN == -1 -> chegou em um nó folha.
    if (current_rrn == -1)
    {
        *promo_key = key;
        *promo_r_child = -1;
        return 1;
    }

    // Lê o nó que o arquivo aponta no momento.
    no_tipo1_t current = read_no_tipo1(findex, current_rrn);

    int pos;

    // Procura nas 3 possíveis chaves do nó lido se o ID já está em uso
    for (pos = 0; pos < current.nroChaves; pos++)
    {
        if (key.id == current.keys[pos].id)
            return -1;

        // Se o nó tiver menos que 3 chaves, sai do loop ao ler presentes.
        if (current.keys[pos].id == -1 || key.id < current.keys[pos].id)
            break;
    }

    key_tipo1_t promo_b_key;
    int promo_b_rrn;
    int return_value = insert_arvb_tipo1(findex, current.filhos[pos], key, &promo_b_key, &promo_b_rrn);

    // Se não for necessário uma promoção, ou se o ID já estiver em uso, retorna o sinal.
    if (return_value == 0 || return_value == -1)
        return return_value;

    // Se houver espaço no registro para a inserção da nova chave
    else if (current.nroChaves < 3)
    {
        // Cria um novo nó zerado
        no_tipo1_t new_no = new_no_arvb_tipo1();

        // Acha pos onde a nova chave vai ser inserida
        int pos;
        int index_new_key;

        for (pos = 0; pos < current.nroChaves; pos++)
        {
            if (current.keys[pos].id == -1 || promo_b_key.id < current.keys[pos].id)
                break;
        }
        index_new_key = pos;

        // Coloca nova chave na pagina nova
        new_no.keys[pos] = promo_b_key;
        new_no.filhos[pos + 1] = promo_b_rrn;
        new_no.nroChaves++;

        // Adiciona as chaves ja existentes na np, pulando a pos da chave nova
        int pos_np = 0;
        for (pos = 0; pos < current.nroChaves; pos++)
        {
            if (pos == index_new_key)
                pos_np++;

            new_no.keys[pos_np] = current.keys[pos];
            new_no.nroChaves++;
            pos_np++;
        }

        // Faz o mesmo das chaves com os filhos
        pos_np = 0;
        for (pos = 0; pos < current.nroChaves + 1; pos++)
        {
            if (pos == index_new_key + 1)
                pos_np++;

            new_no.filhos[pos] = current.filhos[pos];
            pos_np++;
        }

        // Set current = new no
        new_no.tipoNo = current.tipoNo;
        current = new_no;

        if (current.tipoNo != '0' && current.filhos[0] != -1)
            current.tipoNo = '1';

        write_no_arvb_tipo1(findex, current, current_rrn);

        return 0;
    }

    // Caso não haja espaço no registro para a inserção da nova chave, realiza o processo de split.
    else
    {
        no_tipo1_t new_no;

        // Chama a função de split
        split_no_arvb_tipo1(findex, &promo_b_key, &promo_b_rrn, &current, promo_key, promo_r_child, &new_no);

        // Define o tipo do nó
        if (current.filhos[0] != -1)
            current.tipoNo = '1';
        if (new_no.filhos[0] != -1)
            new_no.tipoNo = '1';

        // Escreve no arquivo índice os novos registros
        write_no_arvb_tipo1(findex, current, current_rrn);
        write_no_arvb_tipo1(findex, new_no, *promo_r_child);

        // Atualiza o cabeçalho do arquivo
        setProxRRN_arvb(findex, (*promo_r_child) + 1);
        setNroNos_arvb(findex, getNroNos_arvb(findex) + 1);

        return 1;
    }
}

/*
 * @brief Função que realiza a inserção de um novo registro no arquivo de índices em árvore-B tipo 2.
 */

int insert_arvb_tipo2(FILE *findex, int current_rrn, key_tipo2_t key, key_tipo2_t *promo_key, int *promo_r_child)
{
    // RRN == -1 -> chegou em um nó folha.
    if (current_rrn == -1)
    {
        *promo_key = key;
        *promo_r_child = -1;
        return 1;
    }

    // Lê o nó que o arquivo aponta no momento.
    no_tipo2_t current = read_no_tipo2(findex, current_rrn);

    int pos;

    // Procura nas 3 possíveis chaves do nó lido se o ID já está em uso
    for (pos = 0; pos < current.nroChaves; pos++)
    {
        if (key.id == current.keys[pos].id)
            return -1;

        // Se o nó tiver menos que 3 chaves, sai do loop ao ler presentes.
        if (current.keys[pos].id == -1 || key.id < current.keys[pos].id)
            break;
    }

    key_tipo2_t promo_b_key;
    int promo_b_rrn;
    int return_value = insert_arvb_tipo2(findex, current.filhos[pos], key, &promo_b_key, &promo_b_rrn);

    // Se não for necessário uma promoção, ou se o ID já estiver em uso, retorna o sinal.
    if (return_value == 0 || return_value == -1)
        return return_value;

    // Se houver espaço no registro para a inserção da nova chave
    else if (current.nroChaves < 3)
    {
        // Cria um novo nó zerado
        no_tipo2_t new_no = new_no_arvb_tipo2();

        // Acha pos onde a nova chave vai ser inserida
        int pos;
        int index_new_key;

        for (pos = 0; pos < current.nroChaves; pos++)
        {
            if (current.keys[pos].id == -1 || promo_b_key.id < current.keys[pos].id)
                break;
        }

        index_new_key = pos;

        // Coloca nova chave na pagina nova
        new_no.keys[pos] = promo_b_key;
        new_no.filhos[pos + 1] = promo_b_rrn;
        new_no.nroChaves++;

        // Adiciona as chaves ja existentes na np, pulando a pos da chave nova
        int pos_np = 0;
        for (pos = 0; pos < current.nroChaves; pos++)
        {
            if (pos == index_new_key)
                pos_np++;

            new_no.keys[pos_np] = current.keys[pos];
            new_no.nroChaves++;
            pos_np++;
        }

        // Faz o mesmo das chaves com os filhos
        pos_np = 0;
        for (pos = 0; pos < current.nroChaves + 1; pos++)
        {
            if (pos == index_new_key + 1)
                pos_np++;

            new_no.filhos[pos] = current.filhos[pos];
            pos_np++;
        }

        // Set current = new no
        new_no.tipoNo = current.tipoNo;
        current = new_no;

        if (current.tipoNo != '0' && current.filhos[0] != -1)
            current.tipoNo = '1';

        write_no_arvb_tipo2(findex, current, current_rrn);

        return 0;
    }

    // Caso não haja espaço no registro para a inserção da nova chave, realiza o processo de split.
    else
    {
        no_tipo2_t new_no;

        // Chama a função de split
        split_no_arvb_tipo2(findex, &promo_b_key, &promo_b_rrn, &current, promo_key, promo_r_child, &new_no);

        // Define o tipo do nó
        if (current.filhos[0] != -1)
            current.tipoNo = '1';
        if (new_no.filhos[0] != -1)
            new_no.tipoNo = '1';

        // Escreve no arquivo índice os novos registros
        write_no_arvb_tipo2(findex, current, current_rrn);
        write_no_arvb_tipo2(findex, new_no, *promo_r_child);

        // Atualiza o cabeçalho do arquivo
        setProxRRN_arvb(findex, (*promo_r_child) + 1);
        setNroNos_arvb(findex, getNroNos_arvb(findex) + 1);

        return 1;
    }
}

/*
 * @brief Função que percorre todo o arquivo de dados montando o arquivo de índices em árvore-B para tipo 1 a partir dele
 */

void grava_registro_arvb_tipo1(FILE *fp, FILE *findex)
{
    fseek(fp, END_HEADER1_OFFSET, SEEK_SET);
    fseek(findex, END_HEADER_ARVB_TIPO1_OFFSET, SEEK_SET);

    int id = -1;
    int RRN = -1;

    // Caso não haja nenhum registro no arquivos índice, cria o nó raiz a partir do primeiro registro do arquivo de dados
    if (getNroNos_arvb(findex) == 0)
    {
        // Pega o primeiro ID válido do arquivo de dados
        while (id == -1)
        {
            id = get_prox_id_tipo1(fp);
            RRN++;
        }

        // Cria um nó vazio
        no_tipo1_t raiz = new_no_arvb_tipo1();

        // Seta os valores
        raiz.tipoNo = '0';
        raiz.nroChaves = 1;
        raiz.keys[0].id = id;
        raiz.keys[0].rrn = RRN;

        // Escreve no arquivo
        write_no_arvb_tipo1(findex, raiz, 0);

        // Atualiza o cabeçalho
        setNoRaiz_arvb(findex, 0);
        setNroNos_arvb(findex, 1);
        setProxRRN_arvb(findex, 1);

        RRN++;
    }

    // Recupera o nó raiz da árvore-B
    int root = getNoRaiz_arvb(findex);

    // Prepara as variáveis de promoção
    key_tipo1_t promo_key;
    int promo_r_child;

    // Percorre o arquivo de dados todo
    while (!is_eof(fp))
    {
        // Passa por todos os ID's ignorando os registros logicamente removidos
        id = get_prox_id_tipo1(fp);

        if (id != -1)
        {
            key_tipo1_t key;

            key.id = id;
            key.rrn = RRN;

            // Insere no arquivo índice em árvore-B os ID's
            int return_value = insert_arvb_tipo1(findex, root, key, &promo_key, &promo_r_child);

            if (return_value == 1)
            {
                no_tipo1_t new_root = new_no_arvb_tipo1();
                new_root.tipoNo = '0';
                new_root.nroChaves = 1;
                new_root.keys[0] = promo_key;
                new_root.filhos[0] = root;
                new_root.filhos[1] = promo_r_child;

                int proxRRN = getProxRRN_arvb(findex);
                write_no_arvb_tipo1(findex, new_root, proxRRN);
                setProxRRN_arvb(findex, ++proxRRN);
                setNroNos_arvb(findex, getNroNos_arvb(findex) + 1);

                root = proxRRN - 1;
            }
        }

        RRN++;
    }

    // Atualiza no cabeçalho o nó raiz
    setNoRaiz_arvb(findex, root);
}

/*
 * @brief Função que percorre todo o arquivo de dados montando o arquivo de índices em árvore-B para tipo 2 a partir dele
 */

void grava_registro_arvb_tipo2(FILE *fp, FILE *findex)
{
    fseek(fp, END_HEADER2_OFFSET, SEEK_SET);
    fseek(findex, END_HEADER_ARVB_TIPO2_OFFSET, SEEK_SET);

    int id = -1;
    long long aux;

    // Caso não haja nenhum registro no arquivos índice, cria o nó raiz a partir do primeiro registro do arquivo de dados
    if (getNroNos_arvb(findex) == 0)
    {
        // Pega o primeiro ID válido do arquivo de dados
        while (id == -1)
        {
            id = get_prox_id_tipo2(fp, &aux);
        }

        // Cria um nó vazio
        no_tipo2_t raiz = new_no_arvb_tipo2();

        // Seta os valores
        raiz.tipoNo = '0';
        raiz.nroChaves = 1;
        raiz.keys[0].id = id;
        raiz.keys[0].byteOffset = aux;

        // Escreve no arquivo
        write_no_arvb_tipo2(findex, raiz, 0);

        // Atualiza o cabeçalho
        setNoRaiz_arvb(findex, 0);
        setNroNos_arvb(findex, 1);
        setProxRRN_arvb(findex, 1);
    }

    // Recupera o nó raiz da árvore-B
    int root = getNoRaiz_arvb(findex);

    // Prepara as variáveis de promoção
    key_tipo2_t promo_key;
    int promo_r_child;

    // Percorre o arquivo de dados todo
    while (!is_eof(fp))
    {
        // Passa por todos os ID's ignorando os registros logicamente removidos
        id = get_prox_id_tipo2(fp, &aux);

        if (id != -1)
        {
            key_tipo2_t key;

            key.id = id;
            key.byteOffset = aux;

            // Insere no arquivo índice em árvore-B os ID's
            int return_value = insert_arvb_tipo2(findex, root, key, &promo_key, &promo_r_child);

            if (return_value == 1)
            {
                no_tipo2_t new_root = new_no_arvb_tipo2();
                new_root.tipoNo = '0';
                new_root.nroChaves = 1;
                new_root.keys[0] = promo_key;
                new_root.filhos[0] = root;
                new_root.filhos[1] = promo_r_child;

                int proxRRN = getProxRRN_arvb(findex);
                write_no_arvb_tipo2(findex, new_root, proxRRN);
                setProxRRN_arvb(findex, ++proxRRN);
                setNroNos_arvb(findex, getNroNos_arvb(findex) + 1);

                root = proxRRN - 1;
            }
        }
    }

    // Atualiza no cabeçalho o nó raiz
    setNoRaiz_arvb(findex, root);
}

/*
 * @brief Função que realiza uma busca recursiva por um ID em um arquivo de índices em árvore-B de tipo 1
 */

int search_rrn_by_id_arvb(FILE *findex, int RRN, int to_search_KEY)
{
    // Caso base: RRN == -1 significa que o ID buscado não foi encontrado
    if (RRN == -1)
    {
        return NOT_FOUND;
    }

    else
    {
        // Anda no arquivo até a posição que será realizada a próxima busca
        int pos = 46 + (RRN * 45);
        fseek(findex, pos, SEEK_SET);

        // Recupera o número de chaves do nó atual
        int nro_chaves;
        fread(&nro_chaves, sizeof(int), 1, findex);

        int key, pos_key, page_child;
        pos = pos + 28;

        // Para cada chave presente no nó, é feita a comparação do ID da chave com o ID buscado
        for (int i = 0; i < nro_chaves; i++)
        {
            fread(&key, sizeof(int), 1, findex);
            fread(&pos_key, sizeof(int), 1, findex);

            // Se for o ID buscado, retorna a posição (RRN) em que ele se encontra
            if (key == to_search_KEY)
            {
                return pos_key;
            }

            // Se o ID da chave for maior que a chave buscada, deve-se realizar a busca novamente a partir de um dos filhos do nó atual
            else if (key > to_search_KEY)
            {
                // Para cada situação é feita a analise de onde deverá ser realizada a próxima busca
                if (i == 0)
                {
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }
                else if (i == 1)
                {
                    pos += 4;
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }
                else if (i == 2)
                {
                    pos += 8;
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }

                break;
            }

            // Caso o ID da chave for menor que o ID buscado, e a chave verificada é a última do nó
            else if (key < to_search_KEY && i == nro_chaves - 1)
            {
                pos += nro_chaves * 4;
                fseek(findex, pos, SEEK_SET);
                fread(&page_child, sizeof(int), 1, findex);

                break;
            }
        }

        // Realiza a busca recursivamente pelos filhos dos nós.
        return (search_rrn_by_id_arvb(findex, page_child, to_search_KEY));
    }
}

/*
 * @brief Função que realiza uma busca recursiva por um ID em um arquivo de índices em árvore-B de tipo 2
 */

long search_byteoffset_by_id_arvb(FILE *findex, int RRN, int to_search_KEY)
{
    // Caso base: RRN == -1 significa que o ID buscado não foi encontrado
    if (RRN == -1)
    {
        return NOT_FOUND;
    }

    else
    {
        // Anda no arquivo até a posição que será realizada a próxima busca
        int pos = 58 + (RRN * 57);
        fseek(findex, pos, SEEK_SET);

        // Recupera o número de chaves do nó atual
        int nro_chaves;
        fread(&nro_chaves, sizeof(int), 1, findex);

        int key, pos_key, page_child;
        pos = pos + 36;

        // Para cada chave presente no nó, é feita a comparação do ID da chave com o ID buscado
        for (int i = 0; i < nro_chaves; i++)
        {
            fread(&key, sizeof(int), 1, findex);
            fread(&pos_key, sizeof(long), 1, findex);

            // Se for o ID buscado, retorna a posição (RRN) em que ele se encontra
            if (key == to_search_KEY)
            {
                return pos_key;
            }

            // Se o ID da chave for maior que a chave buscada, deve-se realizar a busca novamente a partir de um dos filhos do nó atual
            else if (key > to_search_KEY)
            {
                // Para cada situação é feita a analise de onde deverá ser realizada a próxima busca
                if (i == 0)
                {
                    pos += 4;
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }
                else if (i == 1)
                {
                    pos += 8;
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }
                else if (i == 2)
                {
                    pos += 12;
                    fseek(findex, pos, SEEK_SET);
                    fread(&page_child, sizeof(int), 1, findex);
                }

                break;
            }

            // Caso o ID da chave for menor que o ID buscado, e a chave verificada é a última do nó
            else if (key < to_search_KEY && i == nro_chaves - 1)
            {
                pos += (nro_chaves + 1) * 4;
                fseek(findex, pos, SEEK_SET);
                fread(&page_child, sizeof(int), 1, findex);

                break;
            }
        }

        // Realiza a busca recursivamente pelos filhos dos nós.
        return (search_byteoffset_by_id_arvb(findex, page_child, to_search_KEY));
    }
}