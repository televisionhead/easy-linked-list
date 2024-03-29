#include "list.h"

int main() {
	//get a list
	list mylist = create_list();

	//define some variables
	int first = 10;
	char* second = "test";
	float third = 100.420f;

	//add them to the list
	mylist.add(&mylist, &first, INT);
	mylist.add(&mylist, &second, STRING);
	mylist.add(&mylist, &third, FLOAT);

	//remove an item
	mylist.remove(&mylist, &first, INT);

	//print list contents
	mylist.print(&mylist);

	//destroy list when done
	mylist.destroy(&mylist);
	
	return 0;
}