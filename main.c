#include "list.h"

/*

TO DO: 

1. split list at certain index ( will be used like this: list secondlist = mylist.split(&mylist); )
2. merge sort a linked list ( will call split with index = midpoint )

*/

int main() {
	list mylist = list_create();

	mylist.destroy(&mylist);
	return 0;
}