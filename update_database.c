#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inverted_search.h"

/* Read the saved (backup) file and put its data into the hash table.
   Lines look like:   #:7     and     Hi:2:file1.txt:6:file2.txt:1:#     */
static int load_backup(FILE *fp, hash_t table[])
{
    char line[1000];
    char *word, *tok, *fname;
    int file_count, count, i;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0')
            continue;

        /* bucket line  "#:7"  -> nothing to do, the bucket is found from the word */
        if (line[0] == '#' && line[1] == ':' && strchr(line + 2, ':') == NULL)
            continue;

        /* word line */
        word = strtok(line, ":");
        tok = strtok(NULL, ":");
        if (word == NULL || tok == NULL || strlen(word) >= WORD_SIZE)
            return FAILURE;
        file_count = atoi(tok);
        if (file_count < 1)
            return FAILURE;

        for (i = 0; i < file_count; i++)
        {
            fname = strtok(NULL, ":");
            tok = strtok(NULL, ":");
            if (fname == NULL || tok == NULL || strlen(fname) >= FNAME_SIZE)
                return FAILURE;
            count = atoi(tok);
            if (count < 1)
                return FAILURE;
            if (insert_word(table, word, fname, count) == FAILURE)
                return FAILURE;
        }
        tok = strtok(NULL, ":");
        if (tok == NULL || strcmp(tok, "#") != 0)   /* every word line ends with :# */
            return FAILURE;
    }
    return SUCCESS;
}

/* Board:  read file name from user
           validate file name (.txt)
           open the file (r mode)
           check: is it a backup file or not
   - backup file     -> load its data into the database
   - normal txt file -> put the name in the file list and index it       */
void update_database(hash_t table[], filenames_t **head)
{
    char name[100];
    FILE *fp;
    int first, i;
    mainnode_t *m;
    subnode_t *s;

    printf("Enter the file name to update data:");
    if (read_line(name, sizeof(name)) == FAILURE)
        return;

    if (check_file(name, *head) == FAILURE)     /* .txt, present, duplicate, empty */
        return;

    fp = fopen(name, "r");                      /* r mode */
    if (fp == NULL)
    {
        printf("Error: %s is not present\n", name);
        return;
    }

    first = fgetc(fp);
    rewind(fp);

    if (first == '#')                           /* backup file: starts with "#:" */
    {
        if (!table_is_empty(table))
            printf("Error: a backup file can be loaded only when the database is empty\n");
        else if (load_backup(fp, table) == FAILURE)
        {
            free_database(table);               /* throw away the half loaded data */
            printf("Error: %s is not a proper backup file, nothing loaded\n", name);
        }
        else
        {
            /* files already inside the backup must not be read again by option 1 */
            for (i = 0; i < SIZE; i++)
                for (m = table[i].link; m != NULL; m = m->link)
                    for (s = m->sublink; s != NULL; s = s->link)
                        remove_file(head, s->f_name);
            printf("Successful : Database updated from backup file: %s\n", name);
        }
        fclose(fp);
    }
    else                                        /* normal text file */
    {
        fclose(fp);
        if (insert_file(head, name) == SUCCESS)
            create_database(*head, table, name);
    }
}
