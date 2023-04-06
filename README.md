## Features
* Easy to use API
* User-defined types
* Multiple types per list
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
mylist.remove(&mylist, &first)

//print list contents
mylist.print(&mylist);

//destroy list when done
mylist.destroy(&mylist);
```
## API
* All available functions and what they do can be found in list.h
* Every function except list_create() takes a reference to a list as the first parameter
* Valid types are UNSPECIFIED, INTEGER, STRING, CHARACTER, DECIMAL_F, DECIMAL_D, STRUCTURE
* Use add() for primitives and add_struct() for structs
* By default, floats and doubles print with a precision of 6 decimal places (use set_precision() to change)

