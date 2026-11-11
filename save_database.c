#include <stdio.h>
#include <string.h>
#include "inverted_search.h"

/* Board: open the backup file (w mode), write the data index by index.
   File format (same as the sample backup file):
       #:7
       Hi:2:file1.txt:6:file2.txt:1:#                                   */
int save_database(hash_t table[], char *name)
{
    FILE *fp;
    int i, len = strlen(name);
    mainnode_t *m;
    subnode_t *s;

    if (len == 0 || len >= FNAME_SIZE)
    {
        printf("Error: invalid file name\n");
        return FAILURE;
    }

    fp = fopen(name, "w");                  /* w mode */
    if (fp == NULL)
    {
        printf("Error: cannot create %s\n", name);
        return FAILURE;
    }

    for (i = 0; i < SIZE; i++)              /* index by index */
    {
        if (table[i].link == NULL)          /* skip empty buckets */
            continue;

        fprintf(fp, "#:%d\n", i);
        for (m = table[i].link; m != NULL; m = m->link)
        {
            fprintf(fp, "%s:%d", m->word, m->file_count);
            for (s = m->sublink; s != NULL; s = s->link)
                fprintf(fp, ":%s:%d", s->f_name, s->word_count);
            fprintf(fp, ":#\n");
        }
    }

    fclose(fp);
    return SUCCESS;
}
