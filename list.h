#ifndef LIST_H
#define LIST_H

typedef struct list {
	//opaque internal list representation (not accessible outside list.c)
	struct list_internal* data;

	//functions we can call on a list externally
	void (*const add)(struct list*, void*, int, void (*)(const void*), int (*comparator)(const void*, const void*)); //add an item to the end of the list
	int (*const remove)(struct list*, void*); //remove first occurence of item based on its data (returns -1 if not found, 0 if successfully removed)
	int (*const remove_all)(struct list*, void*); //remove all occurences of an item with the given data (returns 0 if at least one was removed, else -1)
	struct list (*const split)(struct list*, int); //splits a list in half at the given index and returns a new list (the right half)
	void (*const concat)(struct list*, struct list*); //concatonate two lists, aka first += second, and second remains unchanged
	void (*const sort)(struct list*); //sort the list using merge sort
	void (*const reverse)(struct list*); //reverse the order of the list (first becomes last, etc.)
	int (*const size)(struct list*); //get the number of elements in the list
	void (*const print)(struct list*); //print every element of the list
	void (*const destroy)(struct list*); //free the memory occupied by the list
} list;

//the data types we can store
enum datatype { UNSPECIFIED, INTEGER, STRING, CHARACTER, DECIMAL_F, DECIMAL_D, STRUCTURE };

//get a list
list list_create();

#endif