## Features
* Easy to use API
* User-defined types
* Multiple types per list
* Extensive functionality

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

//print list contents
mylist.print(&mylist);

//destroy list when done
mylist.destroy(&mylist);
```
## API
```c
add(list* mylist, void* item, int datatype) //add a primitive to end of list
add_struct(list* mylist, void* item, int datatype, void (*printer)(const void*), int (*comparator)(const void*, const void*)); //add a struct to end of list

remove_first(list* mylist, void* data) //remove first occurence
remove_all(list* mylist, void* data) //remove all occurences
remove(list* mylist, void* data) //remove first occurence (just calls remove_first)

print(list* mylist) //print contents of list

size(list* mylist) //get number of items in list

sort(list* mylist) //sort list (requires all items to be same type)

split(list* mylist, int index) //split list into two lists at index

concat(list* first, list* second) //concatonate two lists (second remains unchanged, first += second)

reverse(list* mylist) //reverse order of list

setprecision(list* mylist, int precision) //set number of decimal places for printing doubles and floats

destroy(list* mylist) //free memory occupied by list
```
