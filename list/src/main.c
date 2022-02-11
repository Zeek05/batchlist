// List Plugin
// By Zeek Halkyr
// 2/5/22

#include "listlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// developer notes:
// the runaway label is used to break out of the loop and quickly close the file and exit.
// if the list file does not exist, the program will exit with an error. I have added a flag called "/nl" to create a new list, but referencing any file with the format works.

int main(int argc, char** argv) {
    unsigned char exitcode = 0; // will exit with this code
    // if the first argument is -?, --?, /? or ? then print the help message
    if (argc == 2 && (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--?") == 0 || strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "?") == 0)) {
        printf("\nlist.exe by Zeek Halkyr - a list program that can be used to manipulate data\n");
        printf("\n");
        printf("Usage: list [filename] [option] [arguments] [/v] \n\n");
        printf("Lists are stored in a text file that is the first argument. The file is expected to exist, you can create a list with the /nl or new flag.\n");
        printf("The list file will be automatically updated after execution.\n");
        printf("You can specify any file that has strings separate by newlines. Each line is a separate item in the list.\n");
        printf("It is recommended to encapsulate strings in quotes.\n\n");
        printf("Options (batch-style flag | alternative style, parameters are the same): \n");
        printf("\t/v  | verbose - use as the final argument, extends logging level\n");
        printf("\t/af | push <value> - push an item to the front of the list\n"); 
        printf("\t/rf | pop - pop an item from the front of the list and return it\n");
        printf("\t/ab | append <value> - append an item to the end of the list\n");
        printf("\t/rb | popback - pop an item from the end of the list and return it\n");
        printf("\t/ra | remove <index> - remove an item by index and return it\n");
        printf("\t/rw | removewhere <value> - remove an item by value, notifies if not found\n");
        printf("\t/gi | get <index> - print the value stored at an index\n");
        printf("\t/gl | print - print the entire list, each item on a newline\n");
        printf("\t/ia | insert <index> <value> - insert an item at an index, the previous item at that index is pushed to to the right/down\n");
        printf("\t/fv | find <value> - find a value and return its index, notifies if not found\n");
        printf("\t/ll | getlength - get the length of the list in number of elements\n");
        printf("\t/il | sizeof <index> - get the length of a value in the list. returns both characters and bytes size in verbose mode\n");
        printf("\t/nl | new <filename> - new list, creates a new list with the specified name\n\n");
        printf("Examples: \n");
        printf("\tlist.exe list.txt /af \"hello\"\n");
        printf("\tlist.exe list.txt /rf\n");
        printf("\tlist.exe list.txt /gi 3\n");
        printf("\tlist.exe list.txt /il 8\n");
        printf("Exit Codes:\n");
        printf("\t0 - OK\n");
        printf("\t1 - ARG_FAILURE (Missing argument or invalid filename)\n");
        printf("\t2 - EMPTY_LIST (Value not found or the list was empty)\n");
        printf("\t3 - INDEX_OUT_OF_BOUNDS (Negative integer or too large)\n");
        printf("\t4 - FILE_ERROR (Couldn't create a new file, general fault)\n");
        printf("\t5 - NO_COMMAND (Invalid or missing command)\n");
        printf("Notes:\n");
        printf("\tThe program will error if the list file does not exist. Use /nl \n");
        exit(0);
    }
    // check if we have a list file
    node *head = NULL;
    if (argc == 2) {
        // there isn't a command, error
        printf("Error: no command specified, usage: %s <file> [ <command> <args> ] [/v]\n%s /? for help.", argv[0], argv[0]);
        exit(1);
    }
    // if we have more than two arguments:
    if (argc > 2) {
        // if the second argument is /nl we are trying to create a new list, goto escape
        if (strcmp(argv[2], "/nl") == 0 || strcmp(argv[2], "new") == 0) {
            goto escape;
        }
        // else, if the file doesn't exist, report an error and exit
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printf("Error: file %s does not exist.\n", argv[1]);
            exit(1);
        }

    }
    escape:

    // check that we have at least two arguments
    if (argc < 3) {
        printf("Missing argument \"command\", usage: %s <file> [ <command> <args> ] [/v]\n%s /? for help.", argv[0], argv[0]);
        exitcode = 1;
        goto runaway;
    }
    // if the last argument (argv[argc-1]) is "/v", set the verbose flag to true
    int verbose = 0;
    if (strcmp(argv[argc-1], "/v") == 0 || strcmp(argv[2], "new") == 0) {
        verbose = 1;
    }

    // create the list
    head = create_list(argv[1]);
    // switch on the command (the second argument)
    if (strcmp(argv[2], "push") == 0 || strcmp(argv[2], "/af") == 0) {
        // push the third argument to the front of the list
        // if we dont have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"push-item\", Usage: %s <file> [ <command> <args> ]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        push(&head, argv[3]);
        // notify if verbose
        if (verbose) {
            printf("Pushed \"%s\" to the front of the list\n", argv[3]);
        }
        
    }
    else if (strcmp(argv[2], "pop") == 0 || strcmp(argv[2], "/rf") == 0) {
        // pop the first node from the list
        char* value = pop(&head);
        // if the value is null, throw error
        if (value == NULL) {
            printf("List is empty, Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 2;
            goto runaway;
        }
        // notify if verbose
        if (verbose) {
            printf("Popped \"%s\" from the front of the list\n", value);
        }
        // else just print the value
        else {
            printf("%s\n", value);
        }
    }
    else if (strcmp(argv[2], "append") == 0 || strcmp(argv[2], "/ab") == 0) {
        // append an item to the end of the list
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"append-item\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        append(&head, argv[3]);
        // notify if verbose
        if (verbose) {
            printf("Appended \"%s\" to the end of the list\n", argv[3]);
        }

    }
    else if (strcmp(argv[2], "popback") == 0 || strcmp(argv[2], "/rb") == 0 ) {
        // popback an item from the end of the list
        char* value = pop_end(&head);
        // if the value is null, throw error
        if (value == NULL) {
            printf("List is empty, Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        // notify if verbose
        if (verbose) {
            printf("Popped \"%s\" from the end of the list\n", value);
        }
        // else just print the value
        else {
            printf("%s\n", value);
        }
    }
    else if (strcmp(argv[2], "remove") == 0 || strcmp(argv[2], "/ra") == 0) {
        // remove an item from the list at the index in argument 3
        // behavior is undefined for a alphanumeric passed
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"remove-index-#\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        char* value = rem_index(&head, atoi(argv[3]));
        // if the value is null it is out of bounds
        if (value == NULL) {
            printf("Index %i out of bounds, Usage: %s <file> [ <command> <args> ] [/v]\n", atoi(argv[3]), argv[0]);
            exitcode = 3;
            goto runaway;
        }
        // notify if verbose
        if (verbose) {
            printf("Removed \"%s\" from the list at index %i\n", value, atoi(argv[3]));
        }
        // else
        else {
            printf("%s\n", value);
        }
    }
    else if (strcmp(argv[2], "removewhere") == 0 || strcmp(argv[2], "/rw") == 0) {
        // remove an item from the list where the value is the third argument
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"remove-value-string\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        char* value = rem_value(&head, argv[3]);
        // if the value is null it is not in the list or the list is empty
        if (value == NULL) {
            printf("Value \"%s\" not in list.\n", argv[3], argv[0]);
            exitcode = 2;
            goto runaway;
        }
        // notify if verbose
        if (verbose) {
            printf("Removed \"%s\" from the list at %i\n", value, atoi(argv[3]));
        }
        // else
        else {
            printf("%s\n", value);
        }
    }
    else if (strcmp(argv[2], "get") == 0 || strcmp(argv[2], "/gi") == 0) {
        // get a specific index in the list and print it
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"get-index-#\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;      
            goto runaway;
        }
        int l = print_index(head, atoi(argv[3]));
        if (l == -1) {
            printf("Index %i out of bounds (EMPTY_LIST), Usage: %s <file> [ <command> <args> ] [/v]\n", atoi(argv[3]), argv[0]);
            exitcode = 2;
            goto runaway;
        }
        if (l == -2) {
            printf("Index %i out of bounds (NEGATIVE_INDEX), Usage: %s <file> [ <command> <args> ] [/v]\n", atoi(argv[3]), argv[0]);
            exitcode = 3;
            goto runaway;
        }
        if (l == -3) {
            printf("Index %i out of bounds (TOO_BIG - remember the list is zero-indexed), Usage: %s <file> [ <command> <args> ] [/v]\n", atoi(argv[3]), argv[0]);
            exitcode = 3;
            goto runaway;
        }

    }
    else if (strcmp(argv[2], "print") == 0 || strcmp(argv[2], "/gl") == 0) {
        // print the entire list to the screen
        print_list(head);
    }
    else if (strcmp(argv[2], "insert") == 0 || strcmp(argv[2], "/ia") == 0) {
        // insert the item at argument 4 at the index in argument 3
        // if argument 3 is not an integer, behavior is undefined
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"insert-index-#\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        // if we do not have a fourth argument, throw error
        if (argc < 5) {
            printf("Missing argument \"insert-item\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        insert_index(&head, atoi(argv[3]), argv[4]);
        // notify if verbose
        if (verbose) {
            printf("Inserted \"%s\" at index %i\n", argv[4], atoi(argv[3]));
        }
    }
    else if (strcmp(argv[2], "find") == 0 || strcmp(argv[2], "/fv") == 0) {
        // find a value using index_of, the third argument is the value to find
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"find-value-string\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        int index = index_of(head, argv[3]);
        // if the index is -1, the value is not in the list
        if (index == -1) {
            printf("Value \"%s\" not in list.\n", argv[3], argv[0]);    
            exitcode = 2;
            goto runaway;
        }
        // if we are not verbose simply print the index
        if (!verbose) {
            printf("%i\n", index);
        }
        // else print the index and the value
        else {
            printf("%i: %s\n", index, argv[3]);
        }
    }
    else if (strcmp(argv[2], "getlength") == 0 || strcmp(argv[2], "/ll") == 0) {
        // get the length of the entire list
        int le = length(head);
        // if the length is 0, the list is empty
        if (le == 0) {
            exitcode = 2;
        }
        else {
            exitcode = 0;
        }
        // if we are not verbose simply print the length
        if (!verbose) {
            printf("%i\n", le);
        }
        // else print the length of the list in elements and in bytes
        else {
            printf("%i elements, %i bytes\n", le, le * sizeof(struct node));
        }
    }
    else if (strcmp(argv[2], "sizeof") == 0 || strcmp(argv[2], "/il") == 0) {
        // get the size of an item in the list
        // if we do not have a third argument, throw error
        if (argc < 4) {
            printf("Missing argument \"sizeof-index-#\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 1;
            goto runaway;
        }
        int size = value_length(head, atoi(argv[3]));
        // if it returns -1 the list is empty
        if (size == -1) {
            printf("List is empty, Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 2;
            goto runaway;
        }
        // if it returns -2 the index is out of bounds
        if (size == -2) {
            printf("Index %i out of bounds, Usage: %s <file> [ <command> <args> ] [/v]\n", atoi(argv[3]), argv[0]);
            exitcode = 3;
            goto runaway;
        }
        // if we are not verbose simply return the size
        if (!verbose) {
            printf("%i\n", size);
        }
        // else return the size of the item in bytes
        else {
            printf("%i\n%i\n", size, size * 8);
        }
    }
    else if (strcmp(argv[2], "new") == 0 || strcmp(argv[2], "/nl") == 0) {
        // create a new list, the first argument is the file new
        // if the file exists, overwrite it and clear it
        // if the file does not exist, create it
        FILE *fp = fopen(argv[1], "ab+");
        if (fp == NULL) {
            printf("Error creating file, Usage: %s <file> [ <command> <args> ] [/v]\n", argv[0]);
            exitcode = 4;
            goto runaway;
        }
        fclose(fp);
        // notify if verbose
        if (verbose) {
            printf("Created new list file %s\n", argv[1]);
        }


    }
    else {
        printf("Unknown command \"%s\", Usage: %s <file> [ <command> <args> ] [/v]\n", argv[2], argv[0]);
        exitcode = 5;
        goto runaway;
    }
    runaway:
    // write the list to the file
    export_list(head, argv[1]);
    exit(exitcode);

}