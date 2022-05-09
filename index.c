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

#define start_csv_offset 60

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
            inicializa_cabecalho(fp);

            FILE *fcsv = fopen(nome_arquivo_entrada, "rb");
            fseek(fcsv, start_csv_offset, SEEK_SET);

            while(!is_eof(fcsv))
            {
                tipo1_t reg_lido = le_registro_csv(fcsv);
                grava_registro(fp, reg_lido);
            }

            fclose(fcsv);
            fclose(fp);

            binarioNaTela(nome_arquivo_saida);
            
            free(tipo_arquivo);
            free(nome_arquivo_entrada);
            free(nome_arquivo_saida);
            break;

        // mostrar todos os regs (cuidado nao removidos cuidado com lixo)
        case 2:
            char *tipo_arquivo = read_till_delimitor(stdin);
            char *nome_arquivo_entrada = read_till_delimitor(stdin);

            FILE *fp = fopen(nome_arquivo_entrada, "rb");
            
            int count_RRN = 0;
            int ultimo_RRN = get_proxRRN(fp);

            while(count_RRN < ultimo_RRN)
            {
                tipo1_t reg_buscado = busca_prox_registro(fp);
                printa_registro(reg_buscado);
                count_RRN++;  
            }

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

            busca_e_printa_registro_por_parametro(fp, nome_campos, valor_campos, num_campos);

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
            
            int RRN;
            scanf("%d%*c", &RRN);

            tipo1_t reg = busca_registro_por_RRN(fp, RRN);
            printa_registro(reg);

            break;
    }
}
