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
## API Notes
* All available functions and what they do can be found in list.h
* Every function except list_create() takes a reference to a list as the first parameter
* Valid types are UNSPECIFIED, INTEGER, STRING, CHARACTER, DECIMAL_F, DECIMAL_D, STRUCTURE
* Items with UNSPECIFIED type cannot be printed or removed
* Items with STRUCTURE type can only be printed and removed if you pass a printer and comparator function when adding
* Use add() for primitives and add_struct() for structs
* You can only sort() a list if all its elements have the same type and it does not contain any UNSPECIFIED items
* By default, floats and doubles print with a precision of 6 decimal places (use set_precision() to change)
* The split() function mallocs a new list, so don't forget to free it
* When you concat() two lists, the second list is not affected but 
