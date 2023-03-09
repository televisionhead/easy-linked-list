# easy-linked-list
An easy-to-use linked list API written in C with support for primitive types as well as user-defined structs.

# intended use
This project is focused on ease of use and extensibility first and foremost. Use it in an FSF way consistent with the terms of the license if you need an easy to use linked list. It won't be slow by any means, but keep in mind that performance was not the primary objective I had when writing this. In general, linked lists are bad for memory locality, and on top of that I am using void* for generic type support and function pointers for user-friendliness and dynamic dispatch. That being said, unless you're storing a very large number of elements, you should have no problem whatsoever with performance. I will occasionally add more functionality to this project, so please feel free to offer suggestions.

# how to use
Take a look at list.h, as this file contains the commented, externally facing functions and variables that you can use. 

### What's in list.h?
You have the list struct which contains pointers to functions you can perform on a list, and you have the list_create() function for getting a list struct. There is also an enum called datatype, which describes the various data types you can add to a list. You pass one of these to list.add() when you want to add an item. UNSPECIFIED, 0, or NULL indicates that you don't care to provide the type, and you don't intend to print that item or do anything else that requires type information, such as removing based on the underlying data, or retrieving an item based on the underlying data, etc. rather than by the memory address, as these require data type-specific comparisons. Items without type information will also be skipped when calling list.print(). 

### How to print items?
To print a custom type (struct) you have defined, you must provide a printer callback function, rather than NULL, to list.add() (see code below). It is perfectly acceptable for one list to contain multiple items of differing data types, if for some reason you just want one container which you can use to store and perform operations (such as printing) on a bunch of different data types. 

### What's with the struct list_member* data?
The struct list_internal* member of the list struct, while it is in the header, is opaque and not meant to be accessed/changed outside of list.c. It is simply the internal implementation of a linked list and its data, and you call the API functions in order to access and manipulate it. New functionality is added through these function pointer members, which operate on internal data.

### How do I run this program?
If you are new to how compilation works and need help, take a look at compile.sh where you will find the command to compile this module into an executable provided you have the GCC compiler. Optionally add the -DNDEBUG flag (or #define NDEBUG in the code) to disable error-checking. -Ofast makes the compiler optimize the code, -Wall and -Wextra are for general purpose warnings (useful for avoiding bugs).

# Example code from main.c
### User-defined data type and printer function
```
//don't forget to include list.h
#include "list.h"

#include <stdio.h>

//user-defined data type
typedef struct test {
    int first;
    int second;
} test;

//printer callback function which tells the list how to print our user-defined data type
void printer(const void* item) {
    const test* a = (const test*)item;
    printf("struct: first = %d, second = %d\n", a->first, a->second);
}
```

### Initializing a list
```
//initialize a list like this
list mylist = list_create();

//some variables we would like to add to our list
char* one = "test";
test two = { .first = 200, .second = 50 };
double three = 152.5129;
int four = 10;
float five = 89.23f;
```

### Adding items
```
//add them by passing them by reference
//optionally indicate their type, and printer callback function if necessary
mylist.add(&mylist, &one, STRING, NULL);

//note only the struct needs a printer callback in last parameter in order to be printed
mylist.add(&mylist, &two, STRUCTURE, printer);

//note we pass UNSPECIFIED as the type here, so it will not be printed
mylist.add(&mylist, &three, UNSPECIFIED, NULL);

mylist.add(&mylist, &four, INTEGER, NULL);
mylist.add(&mylist, &five, DECIMAL_F, NULL);
```

### Printing, removing, and cleaning up
```
//print the printable items in our list
mylist.print(&mylist);

//remove some items by address
mylist.remove(&mylist, &two);
mylist.remove(&mylist, &five);

//print again
printf("\n");
mylist.print(&mylist);

//don't forget to destroy the list when done
//frees the memory occupied by the internal representation
mylist.destroy(&mylist);
```
