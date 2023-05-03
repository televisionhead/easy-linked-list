## Features
* Easy to use API
* User-defined types
* Multiple types per list
* Printing
* Sorting
* Splitting
* Concatenating
* Reversing
* More!

## Quickstart
```c
//get a list
list mylist = list_create();

//define some variables
int first = 10;
char* second = "test";
float third = 100.420f;

//add them to the list
mylist.add(&mylist, &first, INTEGER);
mylist.add(&mylist, &second, STRING);
mylist.add(&mylist, &third, DECIMAL_F);

//remove an item
mylist.remove(&mylist, &first);

//print list contents
mylist.print(&mylist);

//destroy list when done
mylist.destroy(&mylist);
```
## API Notes
### Overview
* These notes are specific details about usage that may not be obvious, if you just want to learn how to use the API see the examples section
* All available functions (and types) and what they do can be found in list.h
* If you don't care about how I implemented it and you just wanna use my API, all you need to refer to is list.h (if you care, the function pointers in list.h point to equivalent internal functions in list.c, and the internal data of a linked list is contained in struct list_internal* data)
* Every function except list_create() takes a reference to a list as the first parameter

### Types
* Valid types are UNSPECIFIED, INTEGER, STRING, CHARACTER, DECIMAL_F, DECIMAL_D, STRUCTURE
* Items with UNSPECIFIED type cannot be printed or removed
* Items with STRUCTURE type can only be printed and removed if you pass a printer and comparator function to add_struct()
* If you add a STRUCTURE type with add(), the printer and comparator will automatically be NULL
* Instead, use add_struct() for adding structs if you need a printer and/or comparator (you don't have to pass STRUCTURE, too)
* You can only sort() a list if all its elements have the same type, none are UNSPECIFIED, and STRUCTURE types have comparators

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

## Examples
### Get a list
```c
list mylist = list_create() //get a list (don't forget to #include "list.h")
```
### Add a primitive
```c
int ten = 10;
mylist.add(&mylist, &ten, INTEGER); //adds variable ten to the list
```
### Add a struct
```c
struct test {
  int val;
};

void printer(const void* item) {
  struct test* mystruct = (struct test*)item;
  printf("first = %d\n", mystruct->val);
}

int comparator(const void* first, const void* second) {
  return (*(int*)first - *(int*)second);
}

struct test mystruct = { 10 };

mylist.add_struct(&mylist, &mystruct, printer, comparator); //use add_struct(), pass printer and comparator functions
```
### Remove all items of a certain value
```c
mylist.remove(&mylist, &ten); //remove all items with value equal to the value of variable ten
```
### Remove first item of a certain value
```c
mylist.remove_first(&mylist, &ten); //remove first occurence of item with a value equal to the value of variable ten
```
### Print a list
```c
mylist.print(&mylist); //will print all printable items in a list regardless of type unless UNSPECIFIED or STRUCT without printer
```
### Get the number of items
```c
int num_items = mylist.size(&mylist); //num_items will contain the number of elements in mylist
```
### Sort a list
```c
mylist.sort(&mylist); //make sure every element has same type and none are UNSPECIFIED
```
### Split a list
```c
list right = mylist.split(&mylist, 1); //split at index 1, make sure you destroy mylist and right when done
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
