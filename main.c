//don't forget to include list.h
#include "list.h"

#include <stdio.h>

//user-defined data type
typedef struct test {
    int first;
    int second;
} test;

//printer callback function which tells the list how to print our user-defined data type
void printer(const void* item) {
    const test* a = (const test*)item;
    printf("struct: first = %d, second = %d\n", a->first, a->second);
}

int main() {
	//initialize a list like this
	list mylist = list_create();

	//some variables we would like to add to our list
	char* one = "test";
	test two = { .first = 200, .second = 50 };
	double three = 152.5129;
	int four = 10;
	float five = 89.23f;

	//add them by passing them by reference
	//optionally indicating their type and printer callback function if necessary
	mylist.add(&mylist, &one, STRING, NULL);

	//note only the struct needs a printer callback in last parameter in order to be printed
	mylist.add(&mylist, &two, STRUCTURE, printer);

	//note we pass UNSPECIFIED as the type here, so it will not be printed
	mylist.add(&mylist, &three, UNSPECIFIED, NULL);

	mylist.add(&mylist, &four, INTEGER, NULL);
	mylist.add(&mylist, &five, DECIMAL_F, NULL);

	//print the printable items in our list
	mylist.print(&mylist);

	//remove some items by address
	mylist.remove(&mylist, &two);
	mylist.remove(&mylist, &five);

	//print again
	printf("\n");
	mylist.print(&mylist);

	//don't forget to destroy the list when done
	//frees the memory occupied by the internal representation
	mylist.destroy(&mylist);

	return 0;
}