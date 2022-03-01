
// linked list library
// a variety of helper functions for linked lists
// By Zeek Halkyr
// 2/5/22
// "head" is a pointer to the first node in the list
// "value" is the value to be stored in the new node

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// the node has a value and a pointer to the next node
typedef struct node {
    char* value;
    struct node *next;
} node;

// get the length of the list
int length(node *head) {
    // if the list is empty, return 0
    if (head == NULL) {
        return 0;
    }
    // otherwise, find the length
    int length = 0;
    while (head->next != NULL) {
        head = head->next;
        length++;
    }
    return length + 1;
}


// create a new node
node *create_node(char* value) {
    // allocate memory for the node
    node *new_node = (node *) malloc(sizeof(node));
    // set the value of the node, duplicate the string
    new_node->value = strdup(value);
    new_node->next = NULL;
    // return the new node
    return new_node;
}

// append a value to the end of the list
// it takes in a head and the value
// use create_node to create the new node
void append(node **head, char* value) {
    // create a new node with the value
    node *new_node = create_node(value);
    // if the list is empty, set the head to the new node
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    // otherwise, find the last node
    node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    // set the last node's next to the new node
    current->next = new_node;
    return;
}

// add a new node to the beginning of the list
void push(node **head, char* value) {
    // create a new node with the value
    node *new_node = create_node(value);
    // set the new node's next to the current head
    new_node->next = *head;
    // set the head to the new node
    *head = new_node;
}


// remove the first node from the list and return its value
// if the list is empty, return -1
char* pop(node **head) {
    // if the list is empty, return -1
    if (*head == NULL) {
        return NULL;
    }
    // save the value of the first node
    node *temp = *head;
    // set the head to the next node
    *head = (*head)->next;
    char* value = temp->value;
    // free the memory of the first node
    free(temp);
    return value;
}

// create a list from a file
// the file should contain one value per line, separated by newlines
// the newline is removed from the value, we can add it later.
node* create_list(char* filename) {
    // create a new list head
    node *head = NULL;
    // open the file, if it doesnt exist create it
    FILE *file = fopen(filename, "r");
    
    // check that the file opened, if it didnt open return null
    if (file == NULL) {
        return NULL;
    }
    
    // read the file line by line
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        // if there is a newline character at the end of the line, remove it
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        // append the value to the list
        append(&head, line);
    }
    // close the file
    fclose(file);
    // return the list
    return head;
}



// export a list to a file
// the file should contain one value per line, separated by newlines
// the file is automatically cleared on export
void export_list(node* head, char* filename) {
    // open the file
    FILE *file = fopen(filename, "w");

    // check that the file opened
    if (file == NULL) {
        // if it didn't open just return since we ensured it was opened earlier.
        exit(4);
    }


    // write the list to the file
    while (head != NULL) {
        fprintf(file, "%s\n", head->value);
        head = head->next;
    }
    // close the file
    fclose(file);
    return;
}

// remove a list item by index
// if the index is out of bounds, return null
// take in the list head and the index
// it is zero indexed
char* rem_index(node **head, int index) {
    // if the index is out of bounds, return NULL
    if (index < 0) {
        return NULL;
    }
    // if the index is 0, pop the first item
    if (index == 0) {
        return pop(head);
    }
    // if the index is larger than the length of the list, return NULL
    if (index > length(*head)) {
        return NULL;
    }
    // otherwise, find the item at the index
    node *current = *head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    // save the value of the item
    char* value = current->next->value;
    // set the next node to the next node's next lmfao
    current->next = current->next->next;
    // return the value
    return value;
}

// remove a list item by value
// if the value is missing return null
// take in the list head and the value
char* rem_value(node **head, char* value) {
    // if the list is empty, return -1
    if (*head == NULL) {
        return NULL;
    }

    // if there is a newline character at the end of the value, remove it
    if (value[strlen(value) - 1] == '\n') {
        value[strlen(value) - 1] = '\0';
    }

    // if the head's value is the value, pop the first item
    if (strcmp((*head)->value, value) == 0) {
        return pop(head);
    }
    // otherwise, find the item with the value
    node *current = *head;
    while (current->next != NULL) {
        if (strcmp(current->next->value, value) == 0) {
            break;
        }
        current = current->next;
    }
    // if the value is not found, return NULL
    if (current->next == NULL) {
        return NULL;
    }
    // save the value of the item
    char* tvalue = current->next->value;
    // set the next node to the next node's next lmfao
    current->next = current->next->next;
    // return the value
    return tvalue;
}


// remove an item from the end of the list
// it takes in a head
// free the memory of the last node
char* pop_end(node **head) {
    // if the list is empty, return NULL
    if (*head == NULL) {
        return NULL;
    }
    // if the list has one item, pop the first item
    if ((*head)->next == NULL) {
        return pop(head);
    }
    // otherwise, find the last node
    node *current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    // save the value of the last node
    char* value = current->next->value;
    // free the memory of the last node
    free(current->next);
    // set the last node's next to null
    current->next = NULL;
    // return the value
    return value;
}

// return the index of a value in the list
// if it is not found, return -1
// take in the list head and the value
int index_of(node *head, char* value) {
    // if the list is empty, return -1
    if (head == NULL) {
        return -1;
    }
    // if the head's value is the value, return 0
    if (strcmp(head->value, value) == 0) {
        return 0;
    }
    // otherwise, find the item with the value
    node *current = head;
    int index = 0;
    while (current->next != NULL) {
        if (strcmp(current->next->value, value) == 0) {
            return index + 1;
        }
        current = current->next;
        index++;
    }
    // if the value is not found, return -1
    return -1;
}

// print the index specified to the screen
// attach a newline character to the end of the value
int print_index(node *head, int index) {
    // if the list is empty, return
    if (head == NULL) {
        return -1;
    }

    // if the index is out of bounds, return
    if (index < 0) {
        return -2;
    }

    // if the index is 0, print the first item
    if (index == 0) {
        printf("%s\n", head->value);
        return 0;
    }
    // if the index is larger than the length of the list, return
    if (index > length(head) - 1) {
        return -3;
    }
    // otherwise, find the item at the index
    node *current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    // print the value of the item
    printf("%s\n", current->next->value);
    return 0 ;
}

// print the entire list
// attach a newline character to the end of each value
void print_list(node *head) {
    // if the list is empty, return
    if (head == NULL) {
        printf("Empty list.\n");
        return;
    }
    // print the first item
    printf("%s\n", head->value);
    // print the rest of the list
    while (head->next != NULL) {
        head = head->next;
        // if we are at the last item in the list, print the last item without a newline
        if (head->next == NULL) {
            printf("%s", head->value);
        }
        // else print with a newline
        else {
            printf("%s\n", head->value);
        }
    }
    return;
}



// search for an item at the index and return the value length
// if the index is out of bounds, return -1
// take in the list head and the index
// the length of the string does not include the newline character
int value_length(node *head, int index) {
    // if the list is empty, return -1
    if (head == NULL) {
        return -1;
    }
    // if the index is out of bounds, return -1
    if (index < 0) {
        return -2;
    }
    // if the index is 0, return the length of the first item
    if (index == 0) {
        return strlen(head->value);
    }
    // if the index is larger than the length of the list, return -1
    if (index > length(head)) {
        return -1;
    }
    // otherwise, find the item at the index
    node *current = head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    // return the length of the value
    return strlen(current->next->value);
}

// insert a value at the specified index
// the existing value at the index is pushed to the right
// take in the list head, the index, and the value
void insert_index(node **head, int index, char* value) {
    // if the index is out of bounds, return
    if (index < 0) {
        return;
    }
    // if the index is 0, push the first item
    if (index == 0) {
        push(head, value);
        return;
    }

    // if the index is larger than the length of the list, return
    if (index > length(*head)) {
        printf("Could not insert at index %d\n", index);
        return;
    }
    
    // otherwise, find the item at the index
    node *current = *head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }
    // create a new node with the value
    node *new = malloc(sizeof(node));
    new->value = value;
    // set the new node's next to the current node's next
    new->next = current->next;
    // set the current node's next to the new node
    current->next = new;
    return;
}

// reverse a list via recursion
node* reverse(node *head) {
    // if the list is empty, return
    if (head == NULL) {
        return NULL;
    }
    // if the list has one item, return the head
    if (head->next == NULL) {
        return head;
    }
    // otherwise, find the last node
    node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    // save the value of the last node
    char* value = current->next->value;
    // free the memory of the last node
    free(current->next);
    // set the last node's next to null
    current->next = NULL;
    // reverse the list
    node *new = reverse(head);
    // create a new node with the value
    node *new_node = malloc(sizeof(node));
    new_node->value = value;
    // set the new node's next to the new list
    new_node->next = new;
    // return the new node
    return new_node;
}

// define  comparison functions for qsort
// one is for ascending one is for descending sort of integers
int compare_asc(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}
int compare_desc(const void *a, const void *b) {
    return *(int*)b - *(int*)a;
}
// one is for ascending one is for descending sort of character strings
// the longer string is greater than the shorter string
int compare_asc_str(const void *a, const void *b) {
    return strlen(*(char**)a) - strlen(*(char**)b);
}
int compare_desc_str(const void *a, const void *b) {
    return strlen(*(char**)b) - strlen(*(char**)a);
}

// define a function that returns 0 if the string is not a number and 1 if it is
// support multiple digits
int is_number(char *str) {
    // if the string is empty, return 0
    if (str == NULL) {
        return 0;
    }
    // if the string is not empty, check each character
    for (int i = 0; i < strlen(str); i++) {
        // if the character is not a digit, space, newline or tab, return 0
        if (!isdigit(str[i]) && str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            return 0;
        }
    }
    // if the string is a number, return 1
    return 1;
}

// define a sort function, it takes in a list head and a boolean for ascending or descending
// it will sort the list in ascending or descending order using qsort and create a new list containing the sorted values
// if it encounters a non-integer it will return -1
node* sort(node *old_list, int ascending) {
    // if the list is empty return null
    if (old_list == NULL) {
        return NULL;
    }

    // if the list has only one item, return the list
    if (old_list->next == NULL) {
        return old_list;
    }

    // traverse the list and return -1 if there is a non-integer
    node *current = old_list;
    while (current != NULL) {
        if (!is_number(current->value)) {
            return NULL;
        }
        current = current->next;
    }
    // create a new list
    node *new_list = NULL;
    // create an array of integers
    int *array = malloc(sizeof(int) * length(old_list));
    // traverse the list and save the values in the array
    current = old_list;
    for (int i = 0; i < length(old_list); i++) {
        array[i] = atoi(current->value);
        current = current->next;
    }
    // sort the array
    if (ascending) {
        qsort(array, length(old_list), sizeof(int), compare_asc);
    }
    else {
        qsort(array, length(old_list), sizeof(int), compare_desc);
    }
    // traverse the array and create a new list
    for (int i = 0; i < length(old_list); i++) {
        // create a char* for itoa
        char *str = malloc(sizeof(char) * 24);
        append(&new_list, itoa(array[i], str, 10));
    }
    // free the memory of the array
    free(array);
    // return the new list
    return new_list;

}

node* sortstring(node* head, int ascending) {
    // a function that sorts the list based on the length of the strings, it supports alphanumeric characters.
    // it takes in a list head and a boolean for ascending or descending
    // it will sort the list in ascending or descending order using qsort and create a new list containing the sorted values
    // if the list is empty, return null
    if (head == NULL) {
        return NULL;
    }
    // if the list has only one item, return the list
    if (head->next == NULL) {
        return head;
    }
    // create a new list
    node *new_list = NULL;
    // create an array of strings
    char **array = malloc(sizeof(char*) * length(head));
    // traverse the list and save the values in the array
    node *current = head;
    for (int i = 0; i < length(head); i++) {
        array[i] = current->value;
        current = current->next;
    }
    // sort the array
    if (ascending) {
        qsort(array, length(head), sizeof(char*), compare_asc_str);
    }
    else {
        qsort(array, length(head), sizeof(char*), compare_desc_str);
    }
    // traverse the array and create a new list
    for (int i = 0; i < length(head); i++) {
        append(&new_list, array[i]);
    }
    return new_list;
    
}

// 
