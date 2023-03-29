#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

//the number of data types we support
#define NUM_DATATYPES 7

//tolerance value for comparing decimals
#define TOLERANCE 0.000001

//internal link representation
struct link {
	//usual link members
	struct link* prev;
	struct link* next;

	//data, its type, how to print it, and how to compare it
	void* data;
	int datatype;
	void (*printer)(const void*);
	int (*comparator)(const void*, const void*);
};

//internal list representation
struct list_internal {
	//usual list members
	struct link* head;
	struct link* tail;

	//number of items
	int size;
};

//functions for printing various data types
static void no_print(const void* data) { (void)data; } //gets rid of unused parameter warning
static void int_print(const void* data) { printf("%d\n", *(int*)data); }
static void str_print(const void* data) { printf("%s\n", *(char**)data); }
static void char_print(const void* data) { printf("%c\n", *(char*)data); }
static void decimalF_print(const void* data) { printf("%f\n", *(float*)data); }
static void decimalD_print(const void* data) { printf("%f\n", *(double*)data); }

//array of the above printer functions
static void (*const printer_table[NUM_DATATYPES])(const void*) = { no_print, int_print, str_print, char_print, decimalF_print, decimalD_print, no_print };

//functions for comparing various data types (negative value means first < second, positive means first > second, 0 means equal)
static int int_compare(const void* first, const void* second) { return (*(int*)first - *(int*)second); }
static int str_compare(const void* first, const void* second) { return strcmp(*(char**)first, *(char**)second); }
static int char_compare(const void* first, const void* second) { return (*(char*)first - *(char*)second); }
static int float_compare(const void* first, const void* second) { float diff = *(float*)first - *(float*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }
static int double_compare(const void* first, const void* second) { double diff = *(double*)first - *(double*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }

//array of the above comparator functions
static int (*const comparator_table[NUM_DATATYPES])(const void*, const void*) = { NULL, int_compare, str_compare, char_compare, float_compare, double_compare, NULL };

//add an item to the end of the list
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

//find an item based on its data (returns NULL if not found)
static struct link* list_find(struct list_internal* mylist_internal, void* data) {
	assert(mylist_internal != NULL && data != NULL && "NULL passed to list_find()");

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		if(curr->comparator != NULL) {
			if(curr->comparator(curr->data, data) == 0) return curr;
		}

		curr = curr->next;
	}

	return NULL;
}

//remove first occurence of item based on its data (returns -1 if not found, 0 if successfully removed)
static int list_remove_first(list* mylist, void* data) {
	assert(mylist != NULL && data != NULL && "NULL passed to list_remove()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "Cannot remove anything from an empty list!");

	struct link* to_remove;

	if((to_remove = list_find(mylist_internal, data)) == NULL) return -1;

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

//remove all occurences of an item with the given data (returns 0 if at least one was removed, else -1)
static int list_remove_all(list* mylist, void* data) {
	assert(mylist != NULL && data != NULL && "NULL passed to list_remove()");

	int ret;
	if((ret = list_remove_first(mylist, data)) == -1) return -1;

	while(ret == 0 && mylist->data->size != 0) ret = list_remove_first(mylist, data);

	return 0;
}

//get the number of elements in the list
static int list_size(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_size()");

	return mylist->data->size;
}

//print every element of the list
static void list_print(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_print()");

	struct list_internal* mylist_internal = mylist->data;

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		curr->printer(curr->data);
		curr = curr->next;
	}
}

//free the memory occupied by the list
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

//get a list
list list_create() {
	struct list_internal* myinternal = malloc(sizeof(struct list_internal));

	myinternal->head = NULL;
	myinternal->tail = NULL;
	myinternal->size = 0;

	list ret = {
		//internal list representation
		.data = myinternal,

		//API function pointers
		.add = list_add,
		.remove_first = list_remove_first,
		.remove_all = list_remove_all,
		.size = list_size,
		.print = list_print,
		.destroy = list_destroy
	};

	return ret;
}