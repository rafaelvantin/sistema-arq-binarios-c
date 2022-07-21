#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"

#include "../headers/aux_functions.h"

/*
* @brief Avalia os registros a serem printados e printa
*/
void busca_e_printa_registro_por_parametro_tipo1(FILE *fp, char **nome_campos, char **valor_campos, int num_parametros)
{
    tipo1_t registro;

    fseek(fp, END_HEADER1_OFFSET, SEEK_SET);
 
    while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
    {

        if (registro.removido == '0')
        {
            fseek(fp, -1, SEEK_CUR);
            registro = le_registro_dados_tipo1(fp);

            int print = check_condicoes_sao_satisfeitas_tipo1(registro, nome_campos, valor_campos, num_parametros);

            if (print == 1)
            {
                printa_registro_tipo1(registro);
            }

            free(registro.cidade);
            free(registro.marca);
            free(registro.modelo);
        }
        else
        {
            fseek(fp, REGISTER_SIZE - 1, SEEK_CUR);
        }
    }
}

/*
* @brief Dado um rrn, calcula o offset e chama printa_registro_tipo1() 
*/
void busca_e_printa_registro_por_RRN(FILE *fp, int RRN)
{
    long byteoffset = REGISTER_SIZE * (long)RRN;

    fseek(fp, END_HEADER1_OFFSET + byteoffset, SEEK_SET);    

    char removido;

    if ((fread(&removido, sizeof(char), 1, fp) != 0) && removido == '0')
    {
        ungetc(removido, fp);
        tipo1_t registro = le_registro_dados_tipo1(fp);
        printa_registro_tipo1(registro);
    }
    else
    {
        printf("Registro inexistente.\n");
    }
}

/*
* @brief Dado um offset, chama printa_registro_tipo2() 
*/
void busca_e_printa_registro_por_offset(FILE *fp, long long byteoffset)
{
    if(byteoffset == -1)
    {
        printf("Registro inexistente.\n");
        return;
    }

    fseek(fp, byteoffset, SEEK_SET);    

    char removido;

    if ((fread(&removido, sizeof(char), 1, fp) != 0) && removido == '0')
    {
        ungetc(removido, fp);
        tipo2_t registro = le_registro_dados_tipo2(fp);
        printa_registro_tipo2(registro);
    }
    else
    {
        printf("Registro inexistente.\n");
    }
}


/*
* @brief Avalia os registros a serem printados e printa
*/
void busca_e_printa_registro_por_parametro_tipo2(FILE *fp, char **nome_campos, char **valor_campos, int num_parametros)
{
    tipo2_t registro;

    fseek(fp, END_HEADER2_OFFSET, SEEK_SET);
 
    while (fread(&registro.removido, sizeof(char), 1, fp) != 0)
    {
        if (registro.removido == '0')
        {
            fseek(fp, -1, SEEK_CUR);
            registro = le_registro_dados_tipo2(fp);

            int print = check_condicoes_sao_satisfeitas_tipo2(registro, nome_campos, valor_campos, num_parametros);

            if (print == 1)
            {
                printa_registro_tipo2(registro);
            }

            free(registro.cidade);
            free(registro.marca);
            free(registro.modelo);
        }
        else
        {
            fread(&registro.tamanhoRegistro, sizeof(int), 1, fp);
            fseek(fp, registro.tamanhoRegistro - 1, SEEK_CUR);
        }
    }
}
