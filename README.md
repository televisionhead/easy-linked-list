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
* All available functions and what they do can be found in list.h
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
### Adding a struct
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

//use add_struct(), pass printer and comparator functions
mylist.add_struct(&mylist, &mystruct, printer, comparator);
```
### Sorting
```c
mylist.sort(&mylist); //make sure every element has same type and none are UNSPECIFIED
```
### Splitting
```c
list right = mylist.split(&mylist, 1); //split at index 1, make sure you free right when done
```
### Concatenating
```c
mylist.concat(&mylist, &right); //right will be unchanged, mylist += right
```
### Reversing
```c
mylist.reverse(&mylist); //reverse order of elements in list
```
### Setting Precision
```c
mylist.setprecision(&mylist, 3); //floats and doubles will be printed with 3 decimal places
```
