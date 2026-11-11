#include <stdio.h>
#include <string.h>
#include "inverted_search.h"

/* read one line from the keyboard and remove the '\n' */
int read_line(char *buf, int size)
{
    if (fgets(buf, size, stdin) == NULL)
        return FAILURE;                       /* input closed */
    buf[strcspn(buf, "\n")] = '\0';
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    hash_t table[SIZE];
    filenames_t *head = NULL;       /* head of the file name linked list */
    int created = 0;                /* 1 after option 1 is done          */
    int choice, i;
    char line[100], answer = 'y';

    /* command line:  ./a.out f1.txt f2.txt ...  */
    if (argc < 2)
    {
        printf("Usage: %s file1.txt [file2.txt ...]\n", argv[0]);
        return 1;
    }

    /* validate every file name, put the good ones in the linked list */
    for (i = 1; i < argc; i++)
    {
        if (check_file(argv[i], head) == SUCCESS)
            insert_file(&head, argv[i]);
    }
    if (head == NULL)
    {
        printf("No valid file to work with\n");
        return 1;
    }

    init_table(table);

    do
    {
        printf("Select your choice among following options:\n");
        printf("1. Create DATABASE\n");
        printf("2. Display Database\n");
        printf("3. Update DATABASE\n");
        printf("4. Search\n");
        printf("5. Save Database\n");
        printf("Enter your choice\n");

        if (read_line(line, sizeof(line)) == FAILURE)
            break;
        if (sscanf(line, "%d", &choice) != 1)
            choice = 0;                       /* not a number */

        switch (choice)
        {
            case 1:
                if (created)
                    printf("Database is already created\n");
                else
                {
                    create_database(head, table, NULL);
                    created = 1;
                }
                break;

            case 2:
                if (table_is_empty(table))
                    printf("Database is empty, use option 1 first\n");
                else
                    display_database(table);
                break;

            case 3:
                update_database(table, &head);
                break;

            case 4:
                if (table_is_empty(table))
                    printf("Database is empty, use option 1 first\n");
                else
                {
                    printf("Enter the word you want to search: ");
                    read_line(line, sizeof(line));
                    search_database(table, line);
                }
                break;

            case 5:
                if (table_is_empty(table))
                    printf("Database is empty, nothing to save\n");
                else
                {
                    printf("Enter the file name to save database: ");
                    read_line(line, sizeof(line));
                    if (save_database(table, line) == SUCCESS)
                        printf("Database is saved\n");
                }
                break;

            default:
                printf("Invalid choice, enter a number from 1 to 5\n");
                continue;               /* show the menu again */
        }

        do
        {
            printf("Do you want to continue ?\n");
            printf("Enter y/Y to continue and n/N to discontinue\n");
            if (read_line(line, sizeof(line)) == FAILURE)
                line[0] = 'n';
            answer = line[0];
        } while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');

    } while (answer == 'y' || answer == 'Y');

    free_database(table);
    free_file_list(&head);
    return 0;
}
