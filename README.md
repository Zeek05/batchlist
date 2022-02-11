# List.exe - Create and manipulate lists in Batch.
### Introduction
This plugin allows you to create, manipulate and manage lists in CMD for Windows. It uses any file as a list source and updates the list after every operation.
The plugin is flexible, the data created by the list plugin can be used in any other plugin (as it is stored in a file). Each line in a file is an item in the list.
The first item is the list head.

## Installation

Simply download the source which includes a **precompiled list binary**. The plugin requires only one file which is the executable, around 15kb. 
You will call this executable file to operate on the list. It is recommended not to **rename the executable**.

The binary was compiled with an updated version of TinyCC. You can use GCC or any other standard C compiler. There are no POSIX functions used.

## Usage
### Syntax

The plugin allows for basic manipulation as well as some complex commands. More commands will be added in the future.
The syntax of the plugin is `list <listfile> <command> <parameter> </v>`.

`listfile` is the full path to the file where the list will be stored. The file is **expected to exist** and will throw an error if it doesn't. Use the /nl or new command to create a list.
`command` is either the _batch-style flag_ or the _alternative style command-name_ that specifies the operation needed to be done to the list. The flags will be listed below.
`parameter` can be any number of space separated parameters (you can use "" quotations to _separate string parameters_ such as pushing a sentence to the list)
`/v` is an optional flag that can _only be appended to the end of the list_, it extends the logging level of the plugins output.

### Flags

Left, _batch-style flag_. Right, _alternative style_. <_n_> is the name/type of parameter.

/af | push <value> - push an item to the front of the list
/rf | pop - pop an item from the front of the list and return it
/ab | append <value> - append an item to the end of the list
/rb | popback - pop an item from the end of the list and return it
/ra | remove <index> - remove an item by index and return it
/rw | removewhere <value> - remove an item by value, notifies if not found
/gi | get <index> - print the value stored at an index
/gl | print - print the entire list, each item on a newline
/ia | insert <index> <value> - insert an item at an index, the previous item at that index is pushed to to the right/down
/fv | find <value> - find a value and return its index, notifies if not found
/ll | getlength - get the length of the list in number of elements
/il | sizeof <index> - get the length of a value in the list. returns both characters and bytes size in verbose mode
/nl | new <filename> - new list, creates a new list with the specified name.

### Exit Codes
The plugin returns some special exit codes in the case of some errors. They are described below.
0 - OK (No error)
1 - ARG_FAILURE (Missing argument, invalid filename)
2 - EMPTY_LIST (Value wasn't found or the list was empty)
3 - INDEX_OUT_OF_BOUNDS (Negative integer or beyond the length of the list)
4 - FILE_ERROR (Couldn't create a file or had an issue opening it)
5 - NO_COMMAND (Invalid or missing command)

## Example Demonstration
Let's run a batch file that initializes a list of 16 integers, removes the top six, removes the bottom six, and get's the length of the list, and prints the entire list.
The program can easily be run inside of a for loop to **drastically reduce code size** but this is just for simplicity:
	
```
@echo off
color 0f
title Demonstration Program
if exist "demo" del "demo"
list "demo" /nl
list "demo" push "1"
list "demo" push "2"
list "demo" push "3"
list "demo" push "4"
list "demo" push "5"
list "demo" push "6"
list "demo" push "7"
list "demo" push "8"
list "demo" push "9"
list "demo" push "10"
list "demo" push "11"
list "demo" push "12"
list "demo" push "13"
list "demo" push "14"
list "demo" push "15"
list "demo" push "16"
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
echo.
echo Length of list:
list "demo" getlength
echo.
echo Contents of list:
list "demo" print
echo.
pause
```
	
Removing an item from the list returns it to _STDOUT_, so we should get the following:

```
16
15
14
13
12
11
1
2
3
4
5
6

Length of list:
4

Contents of list:
10
9
8
7
Press any key to continue . . .
```
