#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inverted_search.h"

/* Checks in the order written on the board:
   1. extension should be .txt
   2. file is present or not
   3. duplicate file name is present or not
   4. file is empty or not                                   */
int check_file(char *name, filenames_t *head)
{
    FILE *fp;
    filenames_t *p;
    int len = strlen(name);

    /* 1. extension */
    if (len >= FNAME_SIZE)
    {
        printf("Error: file name %s is too long\n", name);
        return FAILURE;
    }
    if (len < 5 || strcmp(name + len - 4, ".txt") != 0)
    {
        printf("Error: %s is not a .txt file\n", name);
        return FAILURE;
    }

    /* 2. present or not */
    fp = fopen(name, "r");
    if (fp == NULL)
    {
        printf("Error: %s is not present\n", name);
        return FAILURE;
    }

    /* 3. duplicate */
    for (p = head; p != NULL; p = p->link)
    {
        if (strcmp(p->filename, name) == 0)
        {
            printf("Error: %s is a duplicate file name\n", name);
            fclose(fp);
            return FAILURE;
        }
    }

    /* 4. empty or not */
    if (fgetc(fp) == EOF)
    {
        printf("Error: %s is empty\n", name);
        fclose(fp);
        return FAILURE;
    }

    fclose(fp);
    return SUCCESS;
}

/* add a file name at the END of the file linked list */
int insert_file(filenames_t **head, char *name)
{
    filenames_t *new_node = calloc(1, sizeof(filenames_t));
    filenames_t *p;

    if (new_node == NULL)
    {
        printf("Error: out of memory\n");
        return FAILURE;
    }
    strcpy(new_node->filename, name);
    new_node->link = NULL;

    if (*head == NULL)                  /* empty list: new node is the head */
        *head = new_node;
    else
    {
        p = *head;
        while (p->link != NULL)         /* go to the last node */
            p = p->link;
        p->link = new_node;
    }
    printf("Successful: inserting file name : %s into file linked list\n", name);
    return SUCCESS;
}

/* delete one name from the list (does nothing if it is not there) */
void remove_file(filenames_t **head, char *name)
{
    filenames_t *p = *head, *prev = NULL;

    while (p != NULL)
    {
        if (strcmp(p->filename, name) == 0)
        {
            if (prev == NULL)
                *head = p->link;        /* it was the first node */
            else
                prev->link = p->link;
            free(p);
            return;
        }
        prev = p;
        p = p->link;
    }
}

void free_file_list(filenames_t **head)
{
    filenames_t *p = *head, *next;

    while (p != NULL)
    {
        next = p->link;
        free(p);
        p = next;
    }
    *head = NULL;
}
