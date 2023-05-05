#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define UPPER_LIMIT 100000000 

void test_add_direct() {
	list mylist = create_list();

	DWORD t1 = GetTickCount();

	for(int i = 0; i < UPPER_LIMIT; i++) {
		int random_number = rand();
		list_add_end(&mylist, &random_number, INTEGER, NULL, NULL);
	}

	DWORD t2 = GetTickCount();
	printf("Time took for %d direct calls: %ld ms\n", UPPER_LIMIT, (t2 - t1));

	mylist.destroy(&mylist);
}

void test_add_indirect() {
	list mylist = create_list();

	DWORD t1 = GetTickCount();

	for(int i = 0; i < UPPER_LIMIT; i++) {
		int random_number = rand();
		mylist.add(&mylist, &random_number, INTEGER);
	}

	DWORD t2 = GetTickCount();
	printf("Time took for %d indirect calls: %ld ms\n", UPPER_LIMIT, (t2 - t1));

	mylist.destroy(&mylist);
}


int main() {
	srand(time(NULL));

	for(int i = 0; i < 3; i++) {
		test_add_direct();
		test_add_indirect();
	}

    return 0;
}