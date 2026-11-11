#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inverted_search.h"

// make every bucket empty (link = NULL, like the board table) 
void init_table(hash_t table[])
{
    int i;
    for (i = 0; i < SIZE; i++)
    {
        table[i].index = i;
        table[i].link = NULL;
    }
}

int table_is_empty(hash_t table[])
{
    int i;
    for (i = 0; i < SIZE; i++)
        if (table[i].link != NULL)
            return 0;
    return 1;
}

// bucket number of a word: letter -> a=0, b=1 ... z=25  (capital letter gives same number)  not letter -> ASCII value of the first character 
int get_index(char *word)
{
    int index;

    if (isalpha((unsigned char)word[0]))
        index = tolower((unsigned char)word[0]) - 'a';
    else
        index = (unsigned char)word[0];

    if (index >= SIZE)
        index = SIZE - 1;
    return index;
}

/* make one sub node */
static subnode_t *make_sub(char *fname, int count)
{
    subnode_t *s = calloc(1, sizeof(subnode_t));

    if (s == NULL)
    {
        printf("Error: out of memory\n");
        return NULL;
    }
    strcpy(s->f_name, fname);
    s->word_count = count;
    s->link = NULL;
    return s;
}

/* Put a word (found in file fname, "count" times) into the database.
   The four cases written on the board:
     1. word same, file name same       -> increase word count
     2. word same, file name different  -> increase file count, create sub node
     3. word different, file different  -> create main node and sub node
     4. word different, file same       -> create main node and sub node      */
int insert_word(hash_t table[], char *word, char *fname, int count)
{
    int index = get_index(word);
    mainnode_t *m = table[index].link, *last = NULL, *new_main;
    subnode_t *s, *slast = NULL;

    /* is the word already in this bucket? */
    while (m != NULL)
    {
        if (strcmp(m->word, word) == 0)
            break;
        last = m;
        m = m->link;
    }

    if (m != NULL)                          /* ---- word same ---- */
    {
        s = m->sublink;
        while (s != NULL)
        {
            if (strcmp(s->f_name, fname) == 0)
            {
                s->word_count += count;     /* case 1 */
                return SUCCESS;
            }
            slast = s;
            s = s->link;
        }
        /* case 2: new file for this word */
        s = make_sub(fname, count);
        if (s == NULL)
            return FAILURE;
        slast->link = s;                    /* add at the end of sub nodes */
        m->file_count++;
        return SUCCESS;
    }

    /* case 3 and 4: new word -> main node + sub node */
    s = make_sub(fname, count);
    if (s == NULL)
        return FAILURE;
    new_main = calloc(1, sizeof(mainnode_t));
    if (new_main == NULL)
    {
        printf("Error: out of memory\n");
        free(s);
        return FAILURE;
    }
    strcpy(new_main->word, word);
    new_main->file_count = 1;
    new_main->sublink = s;
    new_main->link = NULL;

    if (last == NULL)
        table[index].link = new_main;       /* bucket was empty */
    else
        last->link = new_main;              /* add at the end of the bucket */
    return SUCCESS;
}

/* read the files of the list and insert every word.
   only_file == NULL : all files      only_file != NULL : just that file */
int create_database(filenames_t *head, hash_t table[], char *only_file)
{
    filenames_t *p;
    FILE *fp;
    char word[WORD_SIZE];

    for (p = head; p != NULL; p = p->link)
    {
        if (only_file != NULL && strcmp(p->filename, only_file) != 0)
            continue;

        fp = fopen(p->filename, "r");
        if (fp == NULL)
        {
            printf("Error: cannot open %s\n", p->filename);
            continue;
        }

        /* one word at a time (words are separated by space / new line) */
        while (fscanf(fp, "%31s", word) == 1)
            insert_word(table, word, p->filename, 1);

        fclose(fp);
        printf("Successful : Creation of DATABASE for file: %s\n", p->filename);
    }
    return SUCCESS;
}

/* free every main node and its sub nodes */
void free_database(hash_t table[])
{
    int i;
    mainnode_t *m, *mnext;
    subnode_t *s, *snext;

    for (i = 0; i < SIZE; i++)
    {
        m = table[i].link;
        while (m != NULL)
        {
            mnext = m->link;
            s = m->sublink;
            while (s != NULL)
            {
                snext = s->link;
                free(s);
                s = snext;
            }
            free(m);
            m = mnext;
        }
        table[i].link = NULL;
    }
}