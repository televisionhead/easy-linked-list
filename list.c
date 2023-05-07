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

//precision formatting for printing floats and doubles
static char precisionstr[10] = "%.6f\n";

//functions for printing various data types
static void int_print(const void* data) { printf("%d\n", *(int*)data); }
static void str_print(const void* data) { printf("%s\n", *(char**)data); }
static void char_print(const void* data) { printf("%c\n", *(char*)data); }
static void float_print(const void* data) { printf(precisionstr, *(float*)data); }
static void double_print(const void* data) { printf(precisionstr, *(double*)data); }

//array of the above printer functions
static void (*const printer_table[NUM_DATATYPES])(const void*) = { NULL, int_print, str_print, char_print, float_print, double_print, NULL };

//functions for comparing various data types (negative value means first < second, positive means first > second, 0 means equal)
static int int_compare(const void* first, const void* second) { return (*(int*)first - *(int*)second); }
static int str_compare(const void* first, const void* second) { return strcmp(*(char**)first, *(char**)second); }
static int char_compare(const void* first, const void* second) { return (*(char*)first - *(char*)second); }
static int float_compare(const void* first, const void* second) { float diff = *(float*)first - *(float*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }
static int double_compare(const void* first, const void* second) { double diff = *(double*)first - *(double*)second; if(fabs(diff) <= TOLERANCE) return 0; else if(diff > 0) return 1; else return -1; }

//array of the above comparator functions
static int (*const comparator_table[NUM_DATATYPES])(const void*, const void*) = { NULL, int_compare, str_compare, char_compare, float_compare, double_compare, NULL };

//add an item to the end of the list
static void list_add_end(list* mylist, void* item, int datatype, void (*printer)(const void*), int (*comparator)(const void*, const void*)) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_add()");
	assert(datatype >= NONE && datatype <= STRUCT && "datatype passed to list_add() is invalid!");

	struct link* toadd = (struct link*)malloc(sizeof(struct link));
	
	assert(toadd != NULL && "malloc returned NULL in list_add()");

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

//add a primitive to the end of the list. just calls list_add_end with printer and comparator set to NULL 
static void list_add(list* mylist, void* item, int datatype) { list_add_end(mylist, item, datatype, NULL, NULL); }

//add a struct to the end of the list. just calls list_add_end with STRUCT enum and the values you pass for printer and comparator
static void list_add_struct(list* mylist, void* item, void (*printer)(const void*), int (*comparator)(const void*, const void*)) { list_add_end(mylist, item, STRUCT, printer, comparator); }

//find an item based on its data (returns NULL if not found)
static struct link* list_find(struct list_internal* mylist_internal, void* data, int datatype) {	
	assert(mylist_internal != NULL && data != NULL && "NULL passed to list_find()");
	assert(datatype >= NONE && datatype <= STRUCT && "datatype passed to list_find() is invalid!");

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		if(curr->datatype == datatype && curr->comparator != NULL) {
			if(curr->comparator(curr->data, data) == 0) return curr;
		}

		curr = curr->next;
	}

	return NULL;
}

//set the data of the first link with old_data to new_data. returns -1 if link wasnt found, 0 otherwise
static int list_set(list* mylist, void* old_data, void* new_data, int datatype) {
	assert(mylist != NULL && old_data != NULL && new_data != NULL && "NULL passed to list_set()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "Cannot set a value in an empty list!");

	struct link* to_set;
	if((to_set = list_find(mylist_internal, old_data, datatype)) == NULL) return -1;

	to_set->data = new_data;

	return 0;
}

//remove first occurence of item based on its data (returns -1 if not found, 0 if successfully removed)
static int list_remove_first(list* mylist, void* data, int datatype) {	
	assert(mylist != NULL && data != NULL && "NULL passed to list_remove()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "Cannot remove anything from an empty list!");

	struct link* to_remove;

	if((to_remove = list_find(mylist_internal, data, datatype)) == NULL) return -1;

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

//remove all occurences of an item with the given data (returns 0 if one was found and removed, otherwise -1)
static int list_remove_all(list* mylist, void* data, int datatype) {
	assert(mylist != NULL && data != NULL && "NULL passed to list_remove_all()");

	int ret = -1;
	int upper = mylist->data->size;

	for(int i = 0; i < upper; i++) {
		if(mylist->data->size == 0) break;
		if(list_remove_first(mylist, data, datatype) == 0) ret = 0;
	}

	#ifndef NDEBUG
		if(ret == -1) printf("list_remove_all() is returning -1\n");
	#endif

	return ret;
}

//just calls list_remove_all if your too lazy to type .remove_all
static int list_remove(list* mylist, void* data, int datatype) { return list_remove_all(mylist, data, datatype); }

//print every element of the list
static void list_print(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_print()");

	struct link* curr = mylist->data->head;

	while(curr != NULL) {
		if(curr->printer != NULL) curr->printer(curr->data);
		curr = curr->next;
	}
}

//get the number of elements in the list
static int list_size(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_size()");

	return mylist->data->size;
}

//compares the two links and then recursively calls itself
static struct link* list_merge(struct link* firsthead, struct link* secondhead) {
	if(firsthead == NULL) return secondhead;
	if(secondhead == NULL) return firsthead;

	assert(firsthead->datatype == secondhead->datatype && "List has multiple different data types, so it cannot be sorted");
	assert(firsthead->comparator != NULL && "Link with NONE data type has been encountered, so the list cannot be sorted");

	if(firsthead->comparator(firsthead->data, secondhead->data) < 0) {
		firsthead->next = list_merge(firsthead->next, secondhead);
		firsthead->next->prev = firsthead;
		firsthead->prev = NULL;
		return firsthead;
	} else {
		secondhead->next = list_merge(firsthead, secondhead->next);
		secondhead->next->prev = secondhead;
		secondhead->prev = NULL;
		return secondhead;
	}
}

//sort the list using merge sort. list elements must all be the same data type
static void list_sort(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_sort()");

	struct list_internal* mylist_internal = mylist->data;

	int size = mylist_internal->size;

	if(size == 0 || size == 1) return;

	list second = mylist->split(mylist, size / 2);
	list* secondptr = &second;

	list_sort(mylist);
	list_sort(secondptr);

	mylist->data->head = list_merge(mylist->data->head, secondptr->data->head);
	mylist->data->size = size;

	//free the internal representation of second list, but not all the elements
	free(secondptr->data);
}

//splits a list in half at the given index and returns a new list (the right half). dont forget to destroy the new list too when done
static list list_split(list* mylist, int index) {
	assert(mylist != NULL && "NULL passed to list_split()");
	
	struct list_internal* mylist_internal = mylist->data;

	assert(index > 0 && index < mylist_internal->size);

	struct link* curr = mylist_internal->head;

	for(int i = 0; i < index-1; i++) curr = curr->next;

	list ret = create_list();
	ret.data->head = curr->next;
	ret.data->head->prev = NULL;
	ret.data->tail = mylist_internal->tail;
	ret.data->size = mylist_internal->size-index;

	curr->next = NULL;
	mylist_internal->tail = curr;
	mylist_internal->size = index;

	return ret;
}

//concatonate two lists, aka first += second, and second remains unchanged
static void list_concat(list* first, list* second) {
	assert(first != NULL && second != NULL && "NULL passed to list_concat()");

	struct link* curr = second->data->head;

	while(curr != NULL) {
		list_add_end(first, curr->data, curr->datatype, curr->printer, curr->comparator);
		curr = curr->next;
	}
}

//reverse the order of the list (first becomes last, etc.)
static void list_reverse(list* mylist) {
	assert(mylist != NULL && "NULL passed to list_reverse()");

	struct list_internal* mylist_internal = mylist->data;

	if(mylist_internal->size == 0 || mylist_internal->size == 1) return;

	struct link* prev = NULL;
	struct link* next = NULL;
	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	mylist_internal->head = prev;
}

//set the number of decimal places for printing floats and doubles
static void list_setprecision(list* mylist, int precision) {
	assert(mylist != NULL && "NULL passed to list_setprecision()");
	assert(precision > -1 && precision < 21 && "Cannot set precision to negative or greater than 20");

	sprintf(precisionstr, "%%.%df\n", precision);
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
list create_list() {
	struct list_internal* myinternal = malloc(sizeof(struct list_internal));

	assert(myinternal != NULL && "malloc returned NULL in create_list()");

	myinternal->head = NULL;
	myinternal->tail = NULL;
	myinternal->size = 0;

	list ret = {
		//internal list representation
		.data = myinternal,

		//API function pointers
		.add = list_add,
		.add_struct = list_add_struct,
		.set = list_set,
		.remove_first = list_remove_first,
		.remove = list_remove,
		.print = list_print,
		.size = list_size,
		.sort = list_sort,
		.split = list_split,
		.concat = list_concat,
		.reverse = list_reverse,
		.setprecision = list_setprecision,
		.destroy = list_destroy
	};

	return ret;
}