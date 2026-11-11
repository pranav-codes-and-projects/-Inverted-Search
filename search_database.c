#include <stdio.h>
#include <string.h>
#include "inverted_search.h"

void search_database(hash_t table[], char *word)
{
    mainnode_t *m;
    subnode_t *s;

    if (word[0] == '\0')
    {
        printf("Error: nothing to search\n");
        return;
    }

    /* go to the bucket of the word, then walk the main nodes */
    m = table[get_index(word)].link;
    while (m != NULL)
    {
        if (strcmp(m->word, word) == 0)
            break;
        m = m->link;
    }

    if (m == NULL)
    {
        printf("Word %s is not present in the database\n", word);
        return;
    }

    printf("Word %s is present in %d file/s\n", word, m->file_count);
    for (s = m->sublink; s != NULL; s = s->link)
        printf("In file: %s %d time/s\n", s->f_name, s->word_count);
    printf("\n");
}
