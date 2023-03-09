#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PRINTER_TABLE_SIZE 7

//links have their usual members, plus datatype and a printer function pointer
struct link {
	struct link* prev;
	struct link* next;

	void* data;
	int datatype;
	void (*printer)(const void*);
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
static void decimalD_print(const void* data) {printf("%f\n", *(double*)data); }

//table of pointers to our printer functions
static void (*const printer_table[PRINTER_TABLE_SIZE])(const void*) = { no_print, int_print, str_print, char_print, decimalF_print, decimalD_print, no_print };

//add a new item to the list, if datatype is not STRUCT then pass NULL for printer (or if you don't want it to be printed)
static void list_add(list* mylist, void* item, int datatype, void (*printer)(const void*)) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_add()");
	assert(datatype >= UNSPECIFIED && datatype <= STRUCTURE && "datatype passed to list_add() is invalid!");

	struct link* toadd = (struct link*)malloc(sizeof(struct link));
	
	assert(toadd != NULL && "malloc returned NULL! Unable to add item.");

	struct list_internal* mylist_internal = mylist->data;

	toadd->next = NULL;
	toadd->data = item;
	toadd->datatype = datatype;
	toadd->printer = (printer == NULL) ? printer_table[datatype] : printer;

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

//remove the indicated link (find it based on its memory address). success returns 0, not found -1
static int list_remove(list* mylist, void* item) {
	assert(mylist != NULL && item != NULL && "NULL passed to list_remove()");

	struct list_internal* mylist_internal = mylist->data;

	assert(mylist_internal->size != 0 && "Cannot remove anything from an empty list!");

	struct link* curr = mylist_internal->head;

	while(curr != NULL) {
		if(curr->data == item) {
			if(curr == mylist_internal->head) {
				mylist_internal->head = curr->next;
				
				if(mylist_internal->head != NULL) {
					mylist_internal->head->prev = NULL;					
				}
			}
			else if(curr == mylist_internal->tail) {
				mylist_internal->tail = curr->prev;
				mylist_internal->tail->next = NULL;
			}
			else {
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}

			free(curr);
			mylist_internal->size--;
			return 0;
		}

		curr = curr->next;
	}

	return -1;
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
list list_create(void) {
	struct list_internal* myinternal = malloc(sizeof(struct list_internal));

	//normal linked list members not to be visible externally
	myinternal->head = NULL;
	myinternal->tail = NULL;
	myinternal->size = 0;

	//pointers to API functions and the opaque internal list struct
	list ret = {
		.data = myinternal,
		.add = list_add,
		.remove = list_remove,
		.print = list_print,
		.destroy = list_destroy
	};

	return ret;
}