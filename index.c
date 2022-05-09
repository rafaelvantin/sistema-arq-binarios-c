/*
* SCC0215 - Organização de Arquivos
* Trabalho 1 
* 
* Rafael Sartori Vantin
* nUSP: 12543353
*/

#include <stdio.h>
#include <stdlib.h>

#include "tipo1.h"
#include "tipo2.h"
#include "aux_functions.h"

int main()
{
    int operacao;

    scanf("%d%*c", &operacao);

    switch(operacao)
    {
        //escreve tudo do csv
        case 1:
            char *tipo_arquivo = read_till_delimitor(stdin);
            char *nome_arquivo_entrada = read_till_delimitor(stdin);
            char *nome_arquivo_saida = read_till_delimitor(stdin);

            FILE *fp = fopen(nome_arquivo_saida, "wb");

            binarioNaTela(nome_arquivo_saida);
            
            free(tipo_arquivo);
            free(nome_arquivo_entrada);
            free(nome_arquivo_saida);
            break;

        // mostrar todos os regs (cuidado nao removidos cuidado com lixo)
        case 2:
            char *tipo_arquivo = read_till_delimitor(stdin);
            char *nome_arquivo_entrada = read_till_delimitor(stdin);

            free(tipo_arquivo);
            free(nome_arquivo_entrada);
            break;

        // mostra regs WHERE campox = valorx campoy = valory  
        case 3:
            char *tipo_arquivo = read_till_delimitor(stdin);
            char *nome_arquivo_entrada = read_till_delimitor(stdin);
            
            int num_campos;
            scanf("%d%*c", &num_campos);
            char **nome_campos = malloc(num_campos * sizeof(char *));
            char **valor_campos = malloc(num_campos * sizeof(char *));

            for(int i = 0; i < num_campos; i++)
            {
                nome_campos[i] = read_till_delimitor(stdin);
                valor_campos[i] = read_till_delimitor(stdin);
            }

            free(tipo_arquivo);
            free(nome_arquivo_entrada);
            for(int i = 0; i < num_campos; i++)
            {
                free(nome_campos[i]);
                free(valor_campos[i]);
            }
            free(nome_campos);
            free(valor_campos);
            break;

        //mostra regs a partir de rrn
        case 4:
            char *tipo_arquivo = read_till_delimitor(stdin);
            char *nome_arquivo_entrada = read_till_delimitor(stdin);
            
            int rrn;
            scanf("%d%*c", &rrn);

            break;
    }
}
