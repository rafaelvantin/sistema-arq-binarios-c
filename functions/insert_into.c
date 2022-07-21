#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"
#include "../headers/create_bin_file.h"

#include "../headers/aux_functions.h"

int worst_fit_offset(FILE *fp, int proxRegDisponivel)
{
    int tamanhoRegistro, worst_fit_offset = 0, worst_fit_tamanho = 0;

    while (proxRegDisponivel != -1)
    {
        fseek(fp, proxRegDisponivel + 1, SEEK_SET);
        fread(&tamanhoRegistro, sizeof(int), 1, fp);

        if (tamanhoRegistro > worst_fit_tamanho)
        {
            worst_fit_offset = proxRegDisponivel;
            worst_fit_tamanho = tamanhoRegistro;
        }

        fread(&proxRegDisponivel, sizeof(int), 1, fp);
    }

    return worst_fit_offset;
}

void insert_into_tipo_1(FILE *fp, tipo1_t dados)
{
    int topo = get_topo_tipo1(fp);

    if (topo == -1)
    {
        fseek(fp, 0, SEEK_END);
        grava_registro_tipo1(fp, dados);
        atualiza_proxRRN_tipo1(fp, get_proxRRN_tipo1(fp) + 1);
    }
    else
    {
        fseek(fp, END_HEADER1_OFFSET + topo * REGISTER_SIZE + 1, SEEK_SET);
        int novo_topo;
        fread(&novo_topo, sizeof(int), 1, fp);

        fseek(fp, END_HEADER1_OFFSET + topo * REGISTER_SIZE, SEEK_SET);
        grava_registro_tipo1(fp, dados);
        set_topo_tipo1(fp, novo_topo);

        set_nroRegRem_tipo1(fp, get_nroRegRem_tipo1(fp) - 1);
    }
}

void insert_into_tipo_2(FILE *fp, tipo2_t dados)
{
    long long topo = get_topo_tipo2(fp);

    int worstFitOffset = worst_fit_offset(fp, topo);
    int worstFitSize, proxByteOffset;

    fseek(fp, worstFitOffset + 1, SEEK_SET);
    fread(&worstFitSize, sizeof(int), 1, fp);
    fread(&proxByteOffset, sizeof(int), 1, fp);

    if (topo == -1 || dados.tamanhoRegistro > worstFitSize)
    {
        fseek(fp, 0, SEEK_END);
        grava_registro_tipo2(fp, dados);
        atualiza_proxByteOffset(fp, get_proxByteOffset(fp) + dados.tamanhoRegistro + 5);
    }
    else
    {
        fseek(fp, worstFitOffset, SEEK_SET);

        int aux = dados.tamanhoRegistro;
        dados.tamanhoRegistro = worstFitSize;

        grava_registro_tipo2(fp, dados);

        if (worstFitSize > aux)
        {
            for (int i = 0; i < worstFitSize - aux; i++)
            {
                fwrite("$", sizeof(char), 1, fp);
            }
        }

        set_topo_tipo2(fp, proxByteOffset);

        int nroRegRem = get_nroRegRem_tipo2(fp);
        set_nroRegRem_tipo2(fp, nroRegRem - 1);
    }
}