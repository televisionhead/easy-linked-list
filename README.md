## Features
* Easy to use API
* User-defined types
* Multiple types per list
* Extensive functionality

## Quickstart
```c
list mylist = list_create();

int first = 10;
char* second = "test";
float third = 100.420f;

mylist.add(&mylist, &first, INTEGER);
mylist.add(&mylist, &second, STRING);
mylist.add(&mylist, &third, DECIMAL_F);

mylist.print(&mylist);

mylist.destroy(&mylist);
```
