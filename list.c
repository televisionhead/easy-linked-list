#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define NUM_DATATYPES 7
#define TOLERANCE 0.000001

struct link {
	//usual members for link
	struct link* prev;
	struct link* next;

	//actual data, its type, how to print it, and how to compare it to other items
	void* data;
	int datatype;
	void (*printer)(const void*);
	int (*comparator)(const void*, const void*);
};

//internal representation of a list, not to be used externally (only list struct is used outside here)
struct list_internal {
	struct link* head;
	struct link* tail;
	int size;
};

//printer functions for different data types
static void no_print(const void* data) { (void)data; } //stops unused parameter warning
static void int_print(const void* data) { printf("%d\n", *(int*)data); }
static void str_print(const void* data) { printf("%s\n", *(char**)data); }
static void char_print(const void* data) { printf("%c\n", *(char*)data); }
static void decimalF_print(const void* data) { printf("%f\n", *(float*)data); }
static void decimalD_print(const void* data) { printf("%f\n", *(double*)data); }

//comparator functions for different data types. positive return means first > second (or first comes first alphabetically), negative means first < second (or second comes first alphabetically), 0 means first == second. no_compare always returns -1
static int no_compare(const void* first, const void* second) { (void)first; (void)second; return -1; } //stops unused parameter warning. returns -1 because you shouldn't try to compare a no_compare datatype
static int int_compare(const void* first, const void* second) { return (*(int*)first - *(int*)second); }
static int str_compare(const void* first, const void* second) { return strcmp(*(char**)second, *(char**)first); }
static int char_compare(const void* first, const void* second) { return (*(char*)first - *(char*)second); }
static int float_compare(const void* first, const void* second) { float diff = *(float*)first - *(float*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }
static int double_compare(const void* first, const void* second) { double diff = *(double*)first - *(double*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }

//table of pointers to our comparator functions
static int (*const comparator_table[NUM_DATATYPES])(const void*, const void*) = { no_compare, int_compare, str_compare, char_compare, float_compare, double_compare, no_compare };

//table of pointers to our printer functions
static void (*const printer_table[NUM_DATATYPES])(const void*) = { no_print, int_print, str_print, char_print, decimalF_print, decimalD_print, no_print };

//add a new item to the list. pass NULL for printer and comparator if datatype is not STRUCT
static void list_add(list* mylist, void* item, int datatype, void (*printer)(const void*), int (*comparator)(const void*, const void*)) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_add()");
	assert(datatype >= UNSPECIFIED && datatype <= STRUCTURE && "datatype passed to list_add() is invalid!");

	struct link* toadd = (struct link*)malloc(sizeof(struct link));
	
	assert(toadd != NULL && "malloc returned NULL! Unable to add item.");

	struct list_internal* mylist_internal = mylist->data;

	toadd->next = NULL;
	toadd->data = item;
	toadd->datatype = datatype;
	toadd->printer = (printer == NULL) ? printer_table[datatype] : printer;
	toadd->comparator = (comparator == NULL) ? comparator_table[datatype] : comparator;

	if(mylist_internal->head == NULL && mylist_internal->tail == NULL) {
		toadd->prev = NULL;
		mylist_internal->head = toadd;
	}
	else if(mylist_internal->tail == NULL) {
		toadd->prev = mylist_internal->head;
		mylist_internal->head->next = toadd;
		mylist_internal->tail = toadd;
	}
	else {
		toadd->prev = mylist_internal->tail;
		mylist_internal->tail->next = toadd;
		mylist_internal->tail = toadd;
	}

	mylist_internal->size++;
}

//find a link based on the memory address of its data. starts search from head, returns NULL if not found
static struct link* list_find(struct list_internal* mylist_internal, void* item) {
	assert(mylist_internal != NULL && item != NULL && "NULL passed to list_find()");

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		if(curr->data == item) return curr;
		curr = curr->next;
	}

	return NULL;
}

//remove the indicated link (find it based on its memory address). success returns 0, not found -1
static int list_remove(list* mylist, void* item) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_remove()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "Cannot remove anything from an empty list!");

	struct link* to_remove;

	if((to_remove = list_find(mylist_internal, item)) == NULL) return -1;

	if(to_remove == mylist_internal->head) {
		mylist_internal->head = to_remove->next;

		if(mylist_internal->head != NULL) {
			if(mylist_internal->head == mylist_internal->tail) mylist_internal->tail = NULL;

			mylist_internal->head->prev = NULL;
		}
	}
	else if(to_remove == mylist_internal->tail) {
		mylist_internal->tail = to_remove->prev;
		mylist_internal->tail->next = NULL;

		if(mylist_internal->head == mylist_internal->tail) mylist_internal->tail = NULL;
	}
	else {
		to_remove->prev->next = to_remove->next;
		to_remove->next->prev = to_remove->prev;
	}

	free(to_remove);
	mylist_internal->size--;

	return 0;
}

//remove all items with that memory address. returns -1 if none are found, 0 if at least one was found and removed
static int list_remove_all(list* mylist, void* item) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_remove()");

	int ret;
	if((ret = list_remove(mylist, item)) == -1) return -1;

	while(ret == 0 && mylist->data->size != 0) ret = list_remove(mylist, item);

	return 0;
}

//get the number of elements in the list
static int list_size(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_print()");

	return mylist->data->size;
}

//print the data of each link using its printer function pointer
static void list_print(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_print()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "That list has 0 items, so you can't print anything!");

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		curr->printer(curr->data);
		curr = curr->next;
	}
}

//free every link, then the list itself
static void list_destroy(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_destroy()");	

	struct list_internal* mylist_internal = mylist->data;

	struct link* curr = mylist_internal->head->next;

	while(curr != NULL) {
		free(curr->prev);
		curr = curr->next;
	}

	free(mylist_internal->tail);
	free(mylist_internal);
}

//create a list, initialize its members, and return it
list list_create() {
	struct list_internal* myinternal = malloc(sizeof(struct list_internal));

	//normal linked list members not to be visible externally
	myinternal->head = NULL;
	myinternal->tail = NULL;
	myinternal->size = 0;

	list ret = {
		//opaque internal list struct (not visible externally)
		.data = myinternal,

		//API function pointers
		.add = list_add,
		.remove = list_remove,
		.remove_all = list_remove_all,
		.print = list_print,
		.destroy = list_destroy,
		.size = list_size
	};

	return ret;
}