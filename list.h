#ifndef LIST_H
#define LIST_H

typedef struct list {
	//opaque internal list representation (not accessible outside list.c)
	struct list_internal* data;

	//functions we can call on a list externally. each of these points to an equivalent function in list.c (ie. add points to list_add, etc.)
	void (*const add)(struct list*, void*, int); //add a primitive to the end of the list. just calls list_add_end with printer and comparator set to NULL 
	void (*const add_struct)(struct list*, void*, void (*)(const void*), int (*)(const void*, const void*)); //add a struct to the end of the list. just calls list_add_end with STRUCT enum and the values you pass for printer and comparator
	int (*const set)(struct list*, void*, void*, int); //set the data of the first link with old_data to new_data. returns -1 if link wasnt found, 0 otherwise
	int (*const remove_first)(struct list*, void*, int); //remove first occurence of item based on its data (returns -1 if not found, 0 if successfully removed)
	int (*const remove)(struct list*, void*, int); //remove all occurences of an item with the given data (returns 0 if one was found and removed, otherwise -1)
	void (*const print)(struct list*); //print every element of the list
	int (*const size)(struct list*); //get the number of elements in the list
	void (*const sort)(struct list*); //sort the list using merge sort. list elements must all be the same data type
	struct list (*const split)(struct list*, int); //splits a list at the given index and returns a new list (the right half). dont forget to destroy the new list too when done
	void (*const concat)(struct list*, struct list*); //concatonate two lists, aka first += second, and second remains unchanged
	void (*const reverse)(struct list*); //reverse the order of the list (first becomes last, etc.)
	void (*const setprecision)(struct list*, int); //set the number of decimal places for printing floats and doubles
	void (*const destroy)(struct list*); //free the memory occupied by the list
} list;

//the data types we can use externally (note if STRUCT is passed to add(), printer and comparator will be NULL automatically. If you need these, use add_struct() which doesnt require you to pass a data type)
enum datatype { NONE = 0, INT = 1, STRING = 2, CHAR = 3, FLOAT = 4, DOUBLE = 5, STRUCT = 6 };

//get a list
list create_list();

#endif