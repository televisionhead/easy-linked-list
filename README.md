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
