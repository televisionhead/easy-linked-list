#ifndef LIST_H
#define LIST_H

//to use the API, simply include list.h and create a list with list_create
typedef struct list {
	//internal list representation. it is opaque here so that it cannot be accessed/changed externally
	struct list_internal* data;

	//functions pointers which point to functions you can call "on" a list. they're const so no on thinks about changing them externally
	void (*const add)(struct list*, void*, int, void (*)(const void*)); //function pointer-ception. this is for adding an item to a list
	int (*const remove)(struct list*, void*); //this is for removing an item from a list based on its memory address
	void (*const print)(struct list*); //this is for printing the printable items in a list
	void (*const destroy)(struct list*); //this is for freeing the memory occupied by the list's internal representation
} list;

//these are the types you can pass to the list.add() function
enum datatype { UNSPECIFIED, INTEGER, STRING, CHARACTER, DECIMAL_F, DECIMAL_D, STRUCTURE };

//make a list. this is the only function not marked static in list.c, since it is used externally
list list_create(void);

#endif