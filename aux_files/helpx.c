#include <stdio.h>
#include <stdlib.h>
#define SIZE_REGISTRO 238

typedef struct
{
    char firstName[51];
    char lastName[51];
    char email[81];
    char nationality[51];
    int age;
} Registro;

void binarioNaTela(char *nomeArquivoBinario)
{
	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
    {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}


char *readFileName()
{
    char *line = NULL;
    int count = 0;
    
    char aux;

    while(scanf("%c", &aux) != EOF && aux != '\n' && aux != '\r')
    {
        line = realloc(line, sizeof(char)*(count+1));
        line[count] = aux;
        count++;
    }

    line = realloc(line, sizeof(char)*(count + 1));
    line[count] = '\0';

    return line;
}

void readlineAndFormat(char* string, int len)
{
    char c = 0;

    do
    {
        c = (char) getchar();
    } while(c == '\n' || c == '\r');

    int i = 0;

    do
    {
        string[i] = c;
        i++;
        c = getchar();
    } while(c != '\n' && c != '\r');

    string[i]  = '\0';
    i++;

    for(int j = i; j < len; j++)
        string[j] = '$';
}

void printRegistro(Registro reg)
{
    //Flag para registro não existente
    if(reg.age == -1)
        return;

    printf("Firstname: %s\n", reg.firstName);
    printf("Lastname: %s\n", reg.lastName);
    printf("Email: %s\n", reg.email);
    printf("Nationality: %s\n", reg.nationality);
    printf("Age: %d\n\n", reg.age);
}

Registro leRegistro()
{
    Registro novoRegistro;

    readlineAndFormat(novoRegistro.firstName, sizeof(novoRegistro.firstName));
    
    readlineAndFormat(novoRegistro.lastName, sizeof(novoRegistro.lastName));
    
    readlineAndFormat(novoRegistro.email, sizeof(novoRegistro.email));
    
    readlineAndFormat(novoRegistro.nationality, sizeof(novoRegistro.nationality));
    
    scanf("%d%*c", &novoRegistro.age);
    return novoRegistro;
}

Registro buscaRegistro(FILE *fp, int rrn)
{
    Registro reg;

    int byteoffset = rrn * SIZE_REGISTRO;

    fseek(fp, 0, SEEK_END);

    if(byteoffset >= ftell(fp))
    {
        printf("Não foi possível ler o arquivo");
        reg.age = -1;
        return reg;    
    }

    fseek(fp, byteoffset, SEEK_SET);

    fread(reg.firstName, sizeof(char), sizeof(reg.firstName), fp);
    fread(reg.lastName, sizeof(char), sizeof(reg.lastName), fp);
    fread(reg.email, sizeof(char), sizeof(reg.email), fp);    
    fread(reg.nationality, sizeof(char), sizeof(reg.nationality), fp);
    fread(&reg.age, sizeof(int), 1, fp);

    return reg;
}

void gravaRegistro(FILE *fp, Registro reg)
{
    fwrite(reg.firstName, sizeof(char), sizeof(reg.firstName), fp);
    fwrite(reg.lastName, sizeof(char), sizeof(reg.lastName), fp);
    fwrite(reg.email, sizeof(char), sizeof(reg.email), fp);    
    fwrite(reg.nationality, sizeof(char), sizeof(reg.nationality), fp);
    fwrite(&reg.age, sizeof(int), 1, fp);
}

int main()
{
    int operacao;
    char *nomeArquivo;
    FILE *fp;

    scanf("%d%*c", &operacao);
    nomeArquivo = readFileName();

    switch(operacao)
    {
        case 1:

            fp = fopen(nomeArquivo, "ab");

            int qtdRegistros;
            scanf("%d%*c", &qtdRegistros);

            for(int i = 0; i < qtdRegistros; i++)
                gravaRegistro(fp, leRegistro());
            
            fclose(fp);

            binarioNaTela(nomeArquivo);

            break;
        case 2:
            if(!(fp = fopen(nomeArquivo, "rb")))
            {
                printf("Falha no processamento do arquivo");
                break;
            }

            fseek(fp, 0, SEEK_END);
            int fileSize = ftell(fp);
            int numRegs = fileSize / SIZE_REGISTRO; 
            
            for(int i = 0; i < numRegs; i++)
                printRegistro(buscaRegistro(fp, i));

            fclose(fp);

            break;
        case 3:
            if(!(fp = fopen(nomeArquivo, "rb")))
            {
                printf("Falha no processamento do arquivo");
                break;
            }

            int rrn;
            scanf("%d%*c", &rrn);

            printRegistro(buscaRegistro(fp, rrn));

            fclose(fp);
            
            break;
    }

    free(nomeArquivo);
}
