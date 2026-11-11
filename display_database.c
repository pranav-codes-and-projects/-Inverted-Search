#include <stdio.h>
#include "inverted_search.h"

/* board: [Index No] <Word> <file_Count> <file_name> <word_count> */
void display_database(hash_t table[])
{
    int i;
    mainnode_t *m;
    subnode_t *s;

    printf("[index] [word] file_count file/s File: File_name word_count\n");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < SIZE; i++)
    {
        for (m = table[i].link; m != NULL; m = m->link)
        {
            printf("[%d]\t[ %s ] %d file/s:", i, m->word, m->file_count);
            for (s = m->sublink; s != NULL; s = s->link)
                printf(" File : %s %d", s->f_name, s->word_count);
            printf("\n");
        }
    }
    printf("------------------------------------------------------------\n");
}
