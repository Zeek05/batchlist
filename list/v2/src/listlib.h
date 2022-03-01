// header file for listlib.c

#ifndef LISTLIB_H
#define LISTLIB_H

// the node structure
typedef struct node {} node;

// create a node
node *create_node (char* value);

// push a node to the front of the list.
void push (node **head, char* value);

// pop a node from the front of the list and return its value
char* pop (node **head);

// create a list from a file
node* create_list (char* filename);

// export a list
void export_list (node* head, char* filename);

// remove a list item by index
// if the index is out of bounds, return null
// take in the list head and the index
// it is zero indexed
char* rem_index (node **head, int index);

// remove a list item by value
// if the value is missing return null
// take in the list head and the value
char* rem_value (node **head, char* value);

// append 
void append (node **head, char* value);

// pop end
char* pop_end (node **head);

// get the index of a value
int index_of(node *head, char* value);

// print a list item
int print_index(node *head, int index);

// print the entire list
void print_list(node *head);

// get the length of the list
int length(node *head);

// get the length of a value in the list
int value_length(node *head, int index);

void insert_index(node **head, int index, char* value);

// reverse a list
node* reverse(node *head);

// sort a list, 0 ascending 1 descending
node* sort(node *old_list, int ascending);

// sort a list of strings
node* sortstring(node* head, int ascending);

#endif