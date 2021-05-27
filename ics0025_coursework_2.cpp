#include <iostream>
#include <assert.h>
#include "Data.h"


// Test empty container creation
void testEmpty() {
	std::cout << std::endl << "** Test Empty Container **" << std::endl;

	Data data = Data();
	data.PrintAll();

	assert(data.CountItems() == 0);

	std::cout << "** PASS **" << std::endl << std::endl;
}e

/*
Create object containing 300 items.
Use methods PrintAll() and CountItems() to check the created object.
*/
void testCase1() {
	std::cout << std::endl << "** Test Case 1 **" << std::endl;

	int n = 300;
	Data data = Data(n);
	data.PrintAll();

	assert(data.CountItems() == n);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
Select a group.
Condition: there must be at least one subgroup containing at least two items.
Apply methods PrintGroup() and CountGroupItems() for the selected group
*/
void testCase2() {
	std::cout << std::endl << "** Test Case 2 **" << std::endl;

	Data data = Data();

	Date d = Date(1, 1, 2021);

	data.InsertItem('A', 0, "itemA0", d);
	data.InsertItem('B', 0, "itemB0", d);
	data.InsertItem('B', 1, "itemB1", d);

	data.PrintGroup('A');
	data.PrintGroup('B');

	assert(data.CountItems() == 3);
	assert(data.CountGroupItems('A') == 1);
	assert(data.CountGroupItems('B') == 2);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
From the selected group select a subgroup containing at least two items.
Apply methods PrintSubgroupByNames(), PrintSubgroupByDates() and CountSubgroupItems()
for the selected subgroup
*/
void testCase3() {
	std::cout << std::endl << "** Test Case 3 **" << std::endl;

	Data data = Data();

	data.InsertItem('B', 0, "z", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b", Date(1, 3, 2021));
	data.InsertItem('B', 0, "c", Date(1, 2, 2021));

	data.PrintSubgroupByNames('B', 0);
	data.PrintSubgroupByDates('B', 0);

	assert(data.CountSubgroupItems('B', 0) == 3);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
From the selected group select a subgroup containing only one item.
Apply methods PrintSubgroupByNames(), PrintSubgroupByDates() and CountSubgroupItems()
for the selected subgroup.
*/
void testCase4() {
	std::cout << std::endl << "** Test Case 4 **" << std::endl;

	Data data = Data();

	data.InsertItem('B', 0, "z", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b", Date(1, 3, 2021));
	data.InsertItem('B', 0, "c", Date(1, 2, 2021));
	data.InsertItem('B', 1, "d", Date(1, 2, 2021));

	data.PrintSubgroupByNames('B', 1);
	data.PrintSubgroupByDates('B', 1);

	assert(data.CountGroupItems('B') == 4);
	assert(data.CountSubgroupItems('B', 1) == 1);

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
Apply methods PrintSubgroupByNames(), PrintSubgroupByDates() and CountSubgroupItems()
for a non-existing subgroup.
*/
void testCase5() {
	std::cout << std::endl << "** Test Case 5 **" << std::endl;

	Data data = Data();

	data.InsertItem('B', 0, "z", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b", Date(1, 3, 2021));
	data.InsertItem('B', 0, "c", Date(1, 2, 2021));
	data.InsertItem('B', 1, "d", Date(1, 2, 2021));

	try {
		data.PrintSubgroupByNames('B', 5); // non-existing subgroup
		assert(false); // Force test failure if exception is not thrown
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}

	try {
		data.PrintSubgroupByDates('B', 5);  // non-existing subgroup
		assert(false); // Force test failure if exception is not thrown
	}
	catch (const std::exception& e) {
		std::cout << e.what();

		assert(data.CountSubgroupItems('B', 5) == 0);

		std::cout << "** PASS **" << std::endl << std::endl;
	}
}


/*
Apply method PrintItem() for an existing item and for a non-existing item
*/
void testCase6() {
	std::cout << std::endl << "** Test Case 6 **" << std::endl;

	Data data = Data();

	data.InsertItem('A', 0, "a0", Date(1, 1, 2021));

	data.PrintItem('A', 0, "a0");

	try {
		data.PrintItem('A', 0, "NO NAME");  // non-existing item
		assert(false); // Force test failure if exception is not thrown
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		std::cout << "** PASS **" << std::endl << std::endl;
	}
}

/*
Create object containing 30 itemsand apply method PrintAll().
*/
void testCase7() {
	std::cout << std::endl << "** Test Case 7 **" << std::endl;

	Data data = Data(30);
	assert(data.CountItems() == 30);

	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
Apply methods PrintGroup() and CountGroupItems() for a non-existing group
*/
void testCase8() {
	std::cout << std::endl << "** Test Case 8 **" << std::endl;

	Data data = Data();

	data.InsertItem('B', 0, "z", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b", Date(1, 3, 2021));

	try {
		data.PrintGroup('Z'); // non-existing group
		assert(false); // Force test failure if exception is not thrown
	}
	catch (const std::exception& e) {
		std::cout << e.what();

		assert(data.CountGroupItems('Z') == 0);

		std::cout << "** PASS **" << std::endl << std::endl;
	}
}

/*
* Apply method InsertItem() if:
*/
void testCase9() {
	std::cout << std::endl << "** Test Case 9 **" << std::endl;

	Data data = Data();

	// a.The new item will be a member of an existing group and an existing subgroup.
	data.InsertItem('B', 0, "b0", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b1", Date(1, 3, 2021));
	assert(data.CountGroupItems('B') == 2);

	// b.The new item will be a member of an existing group but the subgroup does not exist.
	data.InsertItem('B', 5, "b1", Date(1, 3, 2021));

	assert(data.CountGroupItems('B') == 3);
	assert(data.CountSubgroupItems('B', 5) == 1);

	// c.The new item will be a member of a non - existing group.
	data.InsertItem('C', 0, "c0", Date(1, 3, 2021));
	assert(data.CountItems() == 4);

	// d.This item already exists.
	Item* newItem = data.InsertItem('C', 0, "c0", Date(1, 3, 2021));
	assert(data.CountItems() == 4); // count should remain the same
	assert(!newItem); // should be nullptr

	// e.Apply method PrintAll() to check the results.
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
* Apply method InsertSubgroup() if:
*/
void testCase10() {
	std::cout << std::endl << "** Test Case 10 **" << std::endl;

	Data data = Data();

	// a.The new subgroup will be a member of an existing group.
	data.InsertItem('A', 0, "a0", Date(1, 1, 2021));
	auto newSubgroup1 = data.InsertSubgroup('A', 2, {});
	assert(newSubgroup1); // should not be nullptr

	// b.The new subgroup will be a member of a non - existing group.
	Item* newItem = new Item('B', 0, "b0", Date(1, 1, 2021));
	Item* newItem2 = new Item('B', 0, "b1", Date(1, 1, 2021));
	auto newSubgroup2 = data.InsertSubgroup('B', 0, { newItem, newItem2 });
	assert(newSubgroup2); // should not be nullptr
	assert(data.CountSubgroupItems('B', 0) == 2);

	// c.The subgroup already exists.
	auto newSubgroup3 = data.InsertSubgroup('B', 0, { });
	assert(!newSubgroup3); // should be nullptr
	assert(data.CountSubgroupItems('B', 0) == 2); // existing should not be changed

	// d.Apply method PrintAll() to check the results.
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}


/*
* Apply method InsertGroup() if:
*/
void testCase11() {
	std::cout << std::endl << "** Test Case 11 **" << std::endl;

	Data data = Data();

	// a.The group is new.
	auto newGroup1 = data.InsertGroup('A', {  }, { {  } });
	assert(newGroup1); // should not be nullptr

	// b.The group exists.
	auto newGroup2 = data.InsertGroup('A', {  }, { {  } });
	assert(!newGroup2); // should be nullptr

	// c.Apply method PrintAll() to check the results.
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}


/*
Create object containing 100 items and apply method PrintAll().
*/
void testCase12() {
	std::cout << std::endl << "** Test Case 12 **" << std::endl;

	Data data = Data(100);
	assert(data.CountItems() == 100);

	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
Apply method RemoveItem() so that:
*/
void testCase13() {
	std::cout << std::endl << "** Test Case 13 **" << std::endl;

	Data data = Data();
	data.InsertItem('A', 0, "a00", Date(1, 1, 2021));
	data.InsertItem('A', 0, "a01", Date(1, 1, 2021));
	data.InsertItem('A', 1, "a10", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b00", Date(1, 1, 2021));

	// a.After removing the subgroup does not disappear.
	bool result = data.RemoveItem('A', 0, "a00");
	assert(result); // should succeed
	assert(data.GetSubgroup('A', 0)); // should still exist
	assert(data.GetGroup('A')); // should still exist
	assert(data.CountSubgroupItems('A', 0) == 1); // subgroup count should decrease
	assert(data.CountGroupItems('A') == 2); // group count should decrease

	// b.After removing the subgroup disappears but the group is kept.
	result = data.RemoveItem('A', 0, "a01");
	assert(!data.GetSubgroup('A', 0)); // should not exist anymore
	assert(data.GetGroup('A')); // should still exist
	assert(data.CountGroupItems('A') == 1); // group count should decrease

	// c.After removing the group disappears.
	result = data.RemoveItem('A', 1, "a10");
	assert(!data.GetGroup('A')); // should not exist anymore

	// d.The item to remove does not exist.
	result = data.RemoveItem('B', 0, "NO NAME");
	assert(!result); // should not succeed

	// e.Apply method PrintAll() to check the results
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
Apply method RemoveSubgroup() so that:
*/
void testCase14() {
	std::cout << std::endl << "** Test Case 14 **" << std::endl;

	Data data = Data();
	data.InsertItem('A', 0, "a0", Date(1, 1, 2021));
	data.InsertItem('A', 1, "a1", Date(1, 1, 2021));
	data.InsertItem('B', 0, "b0", Date(1, 1, 2021));

	// a.After removing the group is kept.
	bool result = data.RemoveSubgroup('A', 0);
	assert(result); // should succeed
	assert(data.CountGroupItems('A') == 1); // group count should decrease
	assert(data.GetGroup('A')); // should still exist

	// b.After removing the group disappears.
	result = data.RemoveSubgroup('A', 1);
	assert(result); // should succeed
	assert(!data.GetGroup('A')); // should not exist anymore

	// c.This subgroup does not exist.
	result = data.RemoveSubgroup('B', 10);
	assert(!result); // should not succeed

	// d.Apply method PrintAll() to check the results
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

/*
ApplyMethod RemoveGroup() so that:
*/
void testCase15() {
	std::cout << std::endl << "** Test Case 15 **" << std::endl;

	Data data = Data();
	data.InsertItem('A', 0, "a0", Date(1, 1, 2021));
	data.InsertItem('A', 0, "a1", Date(1, 1, 2021));
	data.InsertItem('A', 2, "a2", Date(1, 1, 2021));

	// a.The group was an existing one.
	bool result = data.RemoveGroup('A');
	assert(result); // should succeed
	assert(!data.GetGroup('A')); // should not exist anymore

	// b.The group does not exist.
	assert(!data.GetGroup('B')); // should not exist
	result = data.RemoveGroup('B');
	assert(!result); // should not succeed

	// e.Apply method PrintAll() to check the results
	data.PrintAll();

	std::cout << "** PASS **" << std::endl << std::endl;
}

int main()
{
	testEmpty();
	testCase1();
	testCase2();
	testCase3();
	testCase4();
	testCase5();
	testCase6();
	testCase7();
	testCase8();
	testCase9();
	testCase10();
	testCase11();
	testCase12();
	testCase13();
	testCase14();
	testCase15();
}
