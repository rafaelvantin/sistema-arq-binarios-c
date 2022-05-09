#include <stdio.h>
#include <stdlib.h>
#include "tipo2.h"

struct cabecalho
{
    char status;
    long long topo;
    char descricao[40];
    char desC1[22];
    char desC2[19];
    char desC3[24];
    char desC4[8];
    char codC5[1];
    char desC5[16];
    char codC6[1];
    char desC6[18];
    char codC7[1];
    char desC7[19];
    long long proxByteOffset;
    int nroRegRem;
};


struct tipo2
{
    int id;
    int ano;
    int qtt;
    char sigla[2];
    char *cidade;
    char *marca;
    char *modelo;
    char removido; //0
    int tamanhoRegistro; //numbytes
    long long prox; //byteoffset do prox removido, -1
    int tamCidade;
    int tamMarca;
    int tamModelo; 
};

cabecalho2_t define_cabecalho()
{
    cabecalho2_t novo;
    novo.status = '1';
    novo.topo = -1;
    strcpy(novo.descricao, "LISTAGEM DA FROTA DOS VEÍCULOS NO BRASIL")
    strcpy(novo.desC1, "CODIGO IDENTIFICADOR: ")
    strcpy(novo.desC2, "ANO DE FABRICACAO: ")
    strcpy(novo.desC3, "QUANTIDADE DE VEICULOS: ")
    strcpy(novo.desC4, "ESTADO: ")
    novo.codC5 = '0';
    strcpy(novo.desC5, "NOME DA CIDADE: ")
    novo.codC6 = '1';
    strcpy(novo.desC6, "MARCA DO VEICULO: ")
    novo.codC7 = '2';
    strcpy(novo.desC7, "MODELO DO VEICULO: ")
    novo.proxByteoffset = 0;
    novo.nroRegRem = 0;
    return novo;

}