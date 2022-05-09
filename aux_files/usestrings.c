#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline()
{
    char *line = NULL;
    int count = 0;
    
    char aux;
    
    while(scanf("%c", &aux) != EOF && aux != '\n')
    {
        if(aux == '\r')
            continue;

        line = realloc(line, sizeof(char)*(count+1));
        line[count] = aux;
        count++;
    }
        
    line = realloc(line, sizeof(char)*(count + 1));
    line[count] = '\0';
    
    return line;
}

char **split_line(int *count, char *line)
{
    char **retValue = NULL;

    char aux = line[0];
    int countWords = 0;
    int countTotalLetters = 0;

    while(1)
    {
        /* Allocate to word till space */
        char *word = NULL;
        int countLetterInWord = 0;

        while(aux != '\0' && aux != ' ')
        {
            word = realloc(word, sizeof(char) * (countLetterInWord + 1));
            word[countLetterInWord] = aux;

            countLetterInWord++;
            aux = line[countTotalLetters + countLetterInWord];
        }
        word = realloc(word, sizeof(char) * (countLetterInWord + 1));
        word[countLetterInWord] = '\0';

        /* Pass string from word to retValue[countWords] */
        retValue = realloc(retValue, sizeof(char*) * (countWords + 1) );
        retValue[countWords] = malloc( sizeof(char) * (countLetterInWord + 1) );
        strcpy(retValue[countWords], word);
        free(word);

        /* Stop condition */   
        if(aux == '\0')
            break;

        countWords++;
        countTotalLetters += countLetterInWord;
        countTotalLetters++;

        aux = line[countTotalLetters];

    }

    *count = countWords;

    return retValue;
}