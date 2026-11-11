#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H

#include <stdio.h>

#define SIZE        128     /* buckets: 0-25 for a-z (board), others for symbols like '?' */
#define WORD_SIZE   32      /* max length of a word (with '\0')       */
#define FNAME_SIZE  32      /* max length of a file name (with '\0')  */

#define SUCCESS 0
#define FAILURE -1

/* 1. file name node (board: "file_name", 2 members) */
typedef struct file
{
    char filename[FNAME_SIZE];
    struct file *link;              /* next file name */
} filenames_t;

/* 4. sub node (board: "Sub node", 3 members) */
typedef struct sub
{
    int word_count;                 /* how many times the word is in this file */
    char f_name[FNAME_SIZE];        /* file name                               */
    struct sub *link;               /* next sub node                           */
} subnode_t;

/* 3. main node (board: "Main node", 4 members)
   board picture:   file_count | word
                    sub link   | main link                                     */
typedef struct mainnode
{
    int file_count;                 /* in how many files the word is present   */
    char word[WORD_SIZE];           /* the word                                */
    struct sub *sublink;            /* "Sub link"  : first sub node            */
    struct mainnode *link;          /* "Main link" : next word in same bucket  */
} mainnode_t;

/* 2. hash table (board: table with columns "Index" and "link", 2 members) */
typedef struct hash
{
    int index;
    mainnode_t *link;               /* first main node of this bucket          */
} hash_t;

/* main.c */
int read_line(char *buf, int size);

/* file_list.c */
int  check_file(char *name, filenames_t *head);
int  insert_file(filenames_t **head, char *name);
void remove_file(filenames_t **head, char *name);
void free_file_list(filenames_t **head);

/* create_database.c */
void init_table(hash_t table[]);
int  table_is_empty(hash_t table[]);
int  get_index(char *word);
int  insert_word(hash_t table[], char *word, char *fname, int count);
int  create_database(filenames_t *head, hash_t table[], char *only_file);
void free_database(hash_t table[]);

/* display_database.c */
void display_database(hash_t table[]);

/* search_database.c */
void search_database(hash_t table[], char *word);

/* save_database.c */
int  save_database(hash_t table[], char *name);

/* update_database.c */
void update_database(hash_t table[], filenames_t **head);

#endif
