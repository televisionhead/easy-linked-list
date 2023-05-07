## Features
* Easy-to-use API
* User-defined types
* Multiple types per list
* Sorting
* Splitting
* Concatenating
* Reversing

## Description
This linked list implementation aims to be easy to use, fast, type-generic, scalable, flexible, maintainable, and feature-rich. Not only does it strike a balance between these, but it excels in all aspects. It achieves dynamic dispatch at runtime through the use of vtables, allowing you to use any primitive or non-primitive data type in a list (you can even have multiple different types in the same list, though you cannot call sort() on such a list).

### Contributing
If you want to contribute a feature or optimization, fix a bug in the code or a problem with my testing methodology, or if you just have a suggestion, feel free to make a pull request or issue. Here is a general guideline for adding a new "method" to the linked list (aka an operation you can do on a list):

1. Define an internal static function in list.c which does the operation (to follow the naming convention, make it start with list_ ie. list_add()). 

2. After you implement your static function, create a function pointer for it in list.h. For naming convention purposes, name it the same as the static function but without list_ (ie. the function pointer with name list() in list.h points to the internal static function list_add() in list.c). This allows your operation to be used by any file which includes list.h (the majority of the externally-facing API consists of these pointers to internal functions). 

3. Assign your internal static function to your external function pointer at the bottom of list.c in the create_list() function (ie. the line .add = list_add() inside the declaration of list ret does this for the add operation). 

After following these three steps, you can #include "list.h" in any compilation unit like main.c and test out your new operation by following the same OOP-style syntax that all the other operations use, for example mylist.add(&mylist, &var, DATATYPE ENUM).

### Performance Considerations
Because linked lists require dynamic memory allocation at runtime, there is bound to be some amount of memory fragmentation which can result in poor memory locality and cache misses for very high n = number of links. Despite this, my testing has showed that this implementation performs quite well despite the indirection I have added to the API through the use of function pointers. In fact, I found that it usually performs better in many cases than directly calling the functions, likely due to the many optimizations GCC does. Even if it didn't use a vtable to achieve dynamic dispatch, it would likely yield similar performance because the overhead it adds is just one index into each vtable (one for printing and one for comparing) when a new link is added. I have found that my several attempts to optimize the code even further had little or no affect, and oftentimes just got in the way of the compiler and made the code slower (probably because the compiler was already doing these optimizations). I'm not a wizard and I certainly didn't try everything, but I would say for the vast majority of use cases this code will be more than fast enough, but I am always open to improvement suggestions. 

### Testing performance
If you want to test out the performance for yourself, checkout the testing branch which has a little demonstration of directly calling list_add_end() versus calling it through the function pointer add() for very high n. For my testing, I always compiled with -Ofast and -static, so I would recommend you do the same if possible. I also used the Windows API function GetTickCount() in the code on the testing branch, so if you are on a Unix-based OS (ie. Linux in most cases, and its many distros) you will need to modify the code a bit to use a timing function from the GNU toolchain. Results may vary depending on your machine, and I only tested the add() function, but I would say in general this implementation is quite fast and I will be using it in larger projects in the future. 

## Quickstart
```c
//get a list
list mylist = create_list();

//define some variables
int first = 10;
char* second = "test";
float third = 100.420f;

//add them to the list
mylist.add(&mylist, &first, INT);
mylist.add(&mylist, &second, STRING);
mylist.add(&mylist, &third, FLOAT);

//remove an item
mylist.remove(&mylist, &first, INT);

//print list contents
mylist.print(&mylist);

//destroy list when done
mylist.destroy(&mylist);
```
## Examples
### Get a list
```c
list mylist = create_list() //get a list (don't forget to #include "list.h")
```
### Add a primitive
```c
int ten = 10;
mylist.add(&mylist, &ten, INT); //adds variable ten to the list
```
### Add a struct
```c
struct test {
  int val;
};

void printer(const void* item) {
  struct test* mystruct = (struct test*)item;
  printf("val = %d\n", mystruct->val);
}

int comparator(const void* first, const void* second) {
  return (*(int*)first - *(int*)second);
}

struct test mystruct = { 10 };

//use add_struct(), pass printer and comparator functions
mylist.add_struct(&mylist, &mystruct, printer, comparator);
```
### Set data of a link
```c
int new = 20;
mylist.set(&mylist, &ten, &new, INT); //set first occurence of value of ten to value of new
```
### Remove all items of a certain value
```c
mylist.remove(&mylist, &ten, INT); //remove all items with same value as ten
```
### Remove first item of a certain value
```c
mylist.remove_first(&mylist, &ten, INT); //remove first item with same value as ten
```
### Print a list
```c
mylist.print(&mylist); //will print all printable items in a list
```
### Get the number of items
```c
int num_items = mylist.size(&mylist); //num_items will contain the number of elements in mylist
```
### Sort a list
```c
mylist.sort(&mylist); //make sure every element has same type and none are NONE
```
### Split a list
```c
list right = mylist.split(&mylist, 3); //split at index 3, make sure you destroy both halves when done
```
### Concatonate a list
```c
mylist.concat(&mylist, &right); //right will be unchanged, mylist += right
```
### Reverse a list
```c
mylist.reverse(&mylist); //reverse order of elements in list
```
### Set printing precision
```c
mylist.setprecision(&mylist, 3); //floats and doubles will be printed with 3 decimal places
```
### Destroy a list
```c
mylist.destroy(&mylist); //always do this when your done to avoid memory leaks
```
## API Notes
### Overview
* These notes are specific details about usage that may not be obvious, if you just want to learn how to use the API see the examples section
* All available functions (and types) and what they do can be found in list.h
* If you don't care about how I implemented it and you just wanna use my API, all you need to refer to is list.h (if you care, the function pointers in list.h point to equivalent internal functions in list.c, and the internal data of a linked list is contained in struct list_internal* data)
* Every function except create_list() takes a reference to a list as the first parameter
* If you don't care about debugging, you can compile with the -DNDEBUG flag: gcc main.c list.c -o main -DNDEBUG
* For new C programmers on Windows: install a GNU toolchain devkit like MinGW-w64 or Cygwin, download/clone this repo and navigate to its directory in the devkit environment/shell, run the compile command (or ./compile.sh), then do ./main to run the code in main.c, which you can modify from here to do whatever you want

### Types
* Valid types are NONE, INT, STRING, CHAR, FLOAT, DOUBLE, and STRUCT
* Items with NONE type cannot be printed or removed
* Items with STRUCT type can only be printed and removed if you pass a printer and comparator function to add_struct()
* If you add a STRUCT type with add(), the printer and comparator will automatically be NULL
* Instead, use add_struct() for adding structs if you need a printer and/or comparator (you don't have to pass STRUCT, too)
* You can only sort() a list if all its elements have the same type, none are NONE, and STRUCT types have comparators
* If you pass the wrong type to a function, you might get unexpected behavior (the operation you tried probably returned -1)

### Printing and comparing structs
* Printer functions return void and take one const void* as a parameter: void (*)(const void*)
* Comparator functions return an int and take two const void* as parameters: int (*)(const void*, const void*)
* You pass these to add_struct() (not add()!) to define how your struct should be printed and/or compared

### Removing
* The remove() function removes all occurences from the list
* If you want to remove just the first occurence, call remove_first()

### Precision
* By default, floats and doubles print with a precision of 6 decimal places (use set_precision() to change)
* Floats and doubles are compared with a precision of 0.000001 (change #define TOLERANCE to a different value if you want)

### Freeing memory
* Always destroy() your list when you are done to free the memory occupied by it
* The split() function mallocs a new list, so don't forget to destroy() it when your done
* When you concat() two lists, the second list is not affected but first += second, so you still need to destroy() both
