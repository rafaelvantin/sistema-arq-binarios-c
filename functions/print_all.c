#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tipo1.h"
#include "../headers/tipo2.h"

#include "../headers/aux_functions.h"

/*
* @brief Printa todos os registros existentes no arquivo tipo1
*/
void printa_todos_registros_tipo1(FILE *fp)
{
    char removido;

    fseek(fp, END_HEADER1_OFFSET, SEEK_SET);

    while (fread(&removido, sizeof(char), 1, fp) != 0)
    {
        ungetc(removido, fp);

        if (removido == '0')
        {
            tipo1_t registro = le_registro_dados_tipo1(fp);
            printa_registro_tipo1(registro);
        }
        else
        {
            fseek(fp, REGISTER_SIZE - 1, SEEK_CUR);
        }
    }

    if (ftell(fp) == END_HEADER1_OFFSET)
    {
        printf("Registro inexistente.\n");
    }
}

/*
* @brief Printa todos os registros existentes no arquivo tipo2
*/
void printa_todos_registros_tipo2(FILE *fp)
{
    char removido;

    fseek(fp, END_HEADER2_OFFSET, SEEK_SET);

    while (fread(&removido, sizeof(char), 1, fp) != 0)
    {
        ungetc(removido, fp);

        if (removido == '0')
        {
            tipo2_t registro = le_registro_dados_tipo2(fp);
            printa_registro_tipo2(registro);
        }
        else
        {
            int tamanhoRegistro;
            fread(&tamanhoRegistro, sizeof(int), 1, fp);
            fseek(fp, tamanhoRegistro - 1, SEEK_CUR);
        }
    }

    if (ftell(fp) == END_HEADER2_OFFSET)
    {
        printf("Registro inexistente.\n");
    }

}