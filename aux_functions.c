#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "headers/aux_functions.h"

/*
* @brief Função para comparação de arquivos binários
*/
void binarioNaTela(char *nomeArquivoBinario)
{
	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
	{
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *)malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for (i = 0; i < fl; i++)
	{
		cs += (unsigned long)mb[i];
	}
	printf("%lf\n", (cs / (double)100));
	free(mb);
	fclose(fs);
}

/*
* @brief Função auxiliar para abrir um ponteiro de arquivo
*/
FILE *open_file(const char *filename, const char *mode, int checkFileConsistent)
{
	FILE *file_pointer;

	file_pointer = fopen(filename, mode);
	if (file_pointer == NULL || (checkFileConsistent && !is_file_consistent(file_pointer)))
	{
		printf("Falha no processamento do arquivo");
		exit(0);
	}

	return file_pointer;
}

/*
* @brief Checa se a string tipo é tipo1
*/
int isTipo1(const char *tipo_arquivo)
{
	return strcmp(tipo_arquivo, "tipo1") == 0 ? 1 : 0;
}

/*
* @brief Checa se a string tipo é tipo2
*/
int isTipo2(const char *tipo_arquivo)
{
	return strcmp(tipo_arquivo, "tipo2") == 0 ? 1 : 0;
}

/*
* @brief Retorna proxRRN do cabecalho (tipo1)
*/
int get_proxRRN_tipo1(FILE *fp)
{
	long antigo = ftell(fp);

	fseek(fp, 174, SEEK_SET);
	int proxRRN;
	fread(&proxRRN, sizeof(int), 1, fp);

	fseek(fp, antigo, SEEK_SET);
	return proxRRN;
}

/*
* @brief Atualiza proxRRN do cabecalho com o valor passado (tipo1)
*/
void atualiza_proxRRN_tipo1(FILE *fp, int proxRRN)
{
	long antigo = ftell(fp);
	fseek(fp, 174, SEEK_SET);
	fwrite(&proxRRN, sizeof(int), 1, fp);
	fseek(fp, antigo, SEEK_SET);
}

/*
* @brief Retorna proxByteOffset do cabeçalho (tipo2)
*/
int get_proxByteOffset(FILE *fp)
{
	long antigo = ftell(fp);

	fseek(fp, 178, SEEK_SET);
	long long proxByteOffset;
	fread(&proxByteOffset, sizeof(long long), 1, fp);

	fseek(fp, antigo, SEEK_SET);
	return proxByteOffset;
}

/*
* @brief Atualiza proxByteOffset do cabeçalho com o valor passado (tipo2)
*/
void atualiza_proxByteOffset(FILE *fp, long long proxByteOffset)
{
	long antigo = ftell(fp);
	fseek(fp, 178, SEEK_SET);
	fwrite(&proxByteOffset, sizeof(long long), 1, fp);
	fseek(fp, antigo, SEEK_SET);
}

/*
* @brief Retorna 'topo' do cabecalho (tipo1)
*/
int get_topo_tipo1(FILE *fp)
{
	long old_offset = ftell(fp);
	fseek(fp, 1, SEEK_SET);

	int topo;
	fread(&topo, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);

	return topo;
}
/*
* @brief Atualiza 'topo' do cabecalho com o valor passado (tipo1)
*/
void set_topo_tipo1(FILE *fp, int topo)
{
	long old_offset = ftell(fp);
	fseek(fp, 1, SEEK_SET);

	fwrite(&topo, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);
}

/*
* @brief Retorna 'topo' do cabecalho (tipo2)
*/
long long get_topo_tipo2(FILE *fp)
{
	long old_offset = ftell(fp);
	fseek(fp, 1, SEEK_SET);

	long long topo;
	fread(&topo, sizeof(long long), 1, fp);

	fseek(fp, old_offset, SEEK_SET);

	return topo;
}

/*
* @brief Atualizado 'topo' do cabecalho com o valor passado (tipo2)
*/
void set_topo_tipo2(FILE *fp, long long topo)
{
	long old_offset = ftell(fp);
	fseek(fp, 1, SEEK_SET);

	fwrite(&topo, sizeof(long long), 1, fp);

	fseek(fp, old_offset, SEEK_SET);
}

/*
* @brief Retorna nroRegRem do cabecalho (tipo1)
*/
int get_nroRegRem_tipo1(FILE *fp)
{
	long old_offset = ftell(fp);
	fseek(fp, 178, SEEK_SET);

	int nroRegRem;
	fread(&nroRegRem, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);

	return nroRegRem;
}

/*
* @brief Atualiza nroRegRem do cabecalho com o valor passado (tipo1)
*/
void set_nroRegRem_tipo1(FILE *fp, int numRegRem)
{
	long old_offset = ftell(fp);
	fseek(fp, 178, SEEK_SET);

	fwrite(&numRegRem, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);
}

/*
* @brief Retorna nroRegRem do cabecalho (tipo1)
*/
int get_nroRegRem_tipo2(FILE *fp)
{
	long old_offset = ftell(fp);
	fseek(fp, 186, SEEK_SET);

	int nroRegRem;
	fread(&nroRegRem, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);

	return nroRegRem;
}

/*
* @brief Atualiza nroRegRem do cabecalho com o valor passado (tipo2)
*/
void set_nroRegRem_tipo2(FILE *fp, int numRegRem)
{
	long old_offset = ftell(fp);
	fseek(fp, 186, SEEK_SET);

	fwrite(&numRegRem, sizeof(int), 1, fp);

	fseek(fp, old_offset, SEEK_SET);
}

int getNoRaiz_arvb(FILE *findex)
{
	long old_offset = ftell(findex);
	fseek(findex, 1, SEEK_SET);

	int noRaiz;
	fread(&noRaiz, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);

	return noRaiz;
}

void setNoRaiz_arvb(FILE *findex, int noRaiz)
{
	long old_offset = ftell(findex);
	fseek(findex, 1, SEEK_SET);

	fwrite(&noRaiz, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);	
}
int getProxRRN_arvb(FILE *findex)
{
	long old_offset = ftell(findex);
	fseek(findex, 5, SEEK_SET);

	int proxRRN;
	fread(&proxRRN, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);

	return proxRRN;
}
void setProxRRN_arvb(FILE *findex, int proxRRN)
{
	long old_offset = ftell(findex);
	fseek(findex, 5, SEEK_SET);

	fwrite(&proxRRN, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);
}
int getNroNos_arvb(FILE *findex)
{
	long old_offset = ftell(findex);
	fseek(findex, 9, SEEK_SET);

	int nroNos;
	fread(&nroNos, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);

	return nroNos;
}
void setNroNos_arvb(FILE *findex, int nroNos)
{
	long old_offset = ftell(findex);
	fseek(findex, 9, SEEK_SET);

	fwrite(&nroNos, sizeof(int), 1, findex);

	fseek(findex, old_offset, SEEK_SET);
}

/*
* @brief Le string até encontrar um desses: EOF, \n, \r, ','
*/
char *read_till_delimitor(FILE *fp)
{
	char *line = NULL;
	int count = 0;

	char aux;

	while (fscanf(fp, "%c", &aux) != EOF && aux != '\n' && aux != '\r' && aux != ',')
	{
		line = realloc(line, sizeof(char) * (count + 1));
		line[count] = aux;
		count++;
	}

	line = realloc(line, sizeof(char) * (count + 1));
	line[count] = '\0';

	return line;
}

/*
* @brief Le a string e conta seus caracteres até encontrar um desses: EOF, \n, \r, ','  
*/
char *read_and_count_till_delimitor(FILE *fp, int *count_words)
{
	char *line = NULL;
	char aux;

	while (fscanf(fp, "%c", &aux) != EOF && aux != '\n' && aux != '\r' && aux != ',')
	{
		line = realloc(line, sizeof(char) * (*count_words + 1));
		line[*count_words] = aux;
		(*count_words)++;
	}

	line = realloc(line, sizeof(char) * (*count_words + 1));
	line[*count_words] = '\0';

	return line;
}

/*
* @brief Le uma palavra do input do usuario
*/
char *read_word(FILE *fp)
{
	char *line = NULL;
	int count = 0;

	char aux;

	fscanf(fp, "%c", &aux);
	while (aux == ' ')
	{
		fscanf(fp, "%c", &aux);
	}
	ungetc(aux, fp);

	while (fscanf(fp, "%c", &aux) != EOF && aux != ' ' && aux != '\n' && aux != '\r' && aux != ',')
	{
		line = realloc(line, sizeof(char) * (count + 1));
		line[count] = aux;
		count++;
	}

	line = realloc(line, sizeof(char) * (count + 2));
	line[count] = '\0';

	return line;
}

/*
* @brief Le uma palavra do input do usuário que está entre aspas e retorna seu conteúdo
*/
char *read_quotes(FILE *fp)
{
	char *line = NULL;
	int count = 0;

	char aux;

	int eof = fscanf(fp, "%c", &aux);
	if (aux == 34)
		eof = fscanf(fp, "%c", &aux);

	while (eof != EOF && aux != '\n' && aux != '\r' && aux != ',')
	{
		line = realloc(line, sizeof(char) * (count + 1));
		line[count] = aux;
		count++;

		eof = fscanf(fp, "%c", &aux);
		if (aux == 34)
			eof = fscanf(fp, "%c", &aux);
	}

	line = realloc(line, sizeof(char) * (count + 1));
	line[count] = '\0';

	return line;
}

char *read_quotes2(FILE *fp)
{
	char *line = NULL;
	int count = 0, stop = 0;

	char aux;

	int eof = fscanf(fp, "%c", &aux);
	while (aux == ' ' || aux == 34)
	{
		eof = fscanf(fp, "%c", &aux);
	}

	while (eof != EOF && aux != '\n' && aux != '\r' && !stop)
	{
		line = realloc(line, sizeof(char) * (count + 1));
		line[count] = aux;
		count++;

		eof = fscanf(fp, "%c", &aux);
		if (aux == 34)
			stop = 1;
		if (count >= 4 && strncmp(line, "NULO", 4) == 0)
		{
			stop = 1;
		}
	}

	// consome o \n ou espaço dps
	if (aux == 34)
		fscanf(fp, "%c", &aux);

	line = realloc(line, sizeof(char) * (count + 1));
	line[count] = '\0';

	return line;
}

/*
* @brief Le uma palavra do input do usuário que está entre aspas e retorna seu conteúdo
*/
char *read_and_count_quotes(FILE *fp, int *count_words)
{
	char *line = NULL;
	int stop = 0;
	*count_words = 0;

	char aux;

	int eof = fscanf(fp, "%c", &aux);
	while (aux == ' ' || aux == 34)
	{
		eof = fscanf(fp, "%c", &aux);
	}

	while (eof != EOF && aux != '\n' && aux != '\r' && aux != ',' && !stop)
	{
		line = realloc(line, sizeof(char) * (*count_words + 1));
		line[*count_words] = aux;
		(*count_words)++;

		eof = fscanf(fp, "%c", &aux);
		if (aux == 34)
			stop = 1;
		if (*count_words >= 4 && strncmp(line, "NULO", 4) == 0)
		{
			stop = 1;
		}
	}

	line = realloc(line, sizeof(char) * (*count_words + 1));
	line[*count_words] = '\0';

	return line;
}

/*
* @brief Checa se o ponteiro do arquivo está no EOF e retorna 1 se sim, 0 se não 
*/
int is_eof(FILE *fp)
{
	int is_eof;
	char aux;
	int antigo = ftell(fp);

	int eof = fread(&aux, sizeof(char), 1, fp);
	while (eof != 0 && (aux == 10 || aux == 13))
	{
		eof = fread(&aux, sizeof(char), 1, fp);
	}

	if (eof == 0)
		is_eof = 1;
	else
		is_eof = 0;

	fseek(fp, -1L, SEEK_CUR);

	return is_eof;
}

/*
* @brief Checa se o arquivo está consistente e retorna 1 se sim, 0 se não
*/
int is_file_consistent(FILE *fp)
{
	char consistent;
	fread(&consistent, sizeof(char), 1, fp);
	ungetc(consistent, fp);
	return consistent - '0';
}

/*
* @brief Atualiza o status do arquivo para o parâmetro passado (0 = inconsistente, 1 = consistente)
*/
void atualiza_status(FILE *fp, char status)
{
	int old_offset = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, fp);
	fseek(fp, old_offset, SEEK_SET);
}