INVERTED SEARCH  (built from the instructor's board pictures)

COMPILE (no Makefile):
    gcc *.c
RUN:
    ./a.out f1.txt
    ./a.out f1.txt f2.txt f3.txt        (you can give many files)
The word to search is NOT given on the command line. Start the program,
then choose option 4 in the menu and type the word.

TEXT FILES PROVIDED (open them with  cat f1.txt  to see the sentences)
    f1.txt   : Apple dog dragon                     (board example)
    f2.txt   : Apple dragon                         (board example)
    f3.txt   : 2 sentences, some repeated words
    file1.txt, file2.txt : the files of the sample output
    sample_backup.txt    : the backup file of the sample output
You can make more:   echo "my own sentence here" > f4.txt

TRY THE BOARD EXAMPLE:
    ./a.out f1.txt
    1  y            (create database from f1.txt)
    3  f2.txt  y    (update: add f2.txt)
    2  y            (display)
 you get:
    [0]  [ Apple ] 2 file/s: File : f1.txt 1 File : f2.txt 1
    [3]  [ dog ] 1 file/s: File : f1.txt 1
    [3]  [ dragon ] 2 file/s: File : f1.txt 1 File : f2.txt 1

TRY SAVE + BACKUP:
    option 5, type backup.txt  -> file is written index by index
    run again:  ./a.out f3.txt
    option 3, type backup.txt  -> the backup file is loaded into the database
    (a backup file can be loaded only while the database is empty)

FILES IN THE CODE
    inverted_search.h    the 4 structures + all function names
    main.c               command line, file name checks, menu, y/n question
    file_list.c          check_file (the 4 checks), file name linked list
    create_database.c    hash table, insert_word (the 4 cases), read the files
    display_database.c   option 2
    search_database.c    option 4
    save_database.c      option 5  (open w mode, write index by index)
    update_database.c    option 3  (text file -> index it, backup file -> load it)
