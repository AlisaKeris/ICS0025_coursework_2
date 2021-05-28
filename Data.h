#pragma once

#include <map>
#include <list>
#include <initializer_list>
#include "Item.h";

class Data
{
private:
	// ** Interpretation of the type **
	// map with:
	//			key = char
	//			value = pointer to map with:
	//										key = int
	//										value = pointer to list of pointers to Item
	std::map<char, std::map<int, std::list<Item*>*>*> DataStructure;

public:
	Data();
	Data(int n);
	~Data();

	void PrintAll();
	int CountItems();

	std::map<int, std::list<Item*>*>* GetGroup(char c);
	void PrintGroup(char c);
	int CountGroupItems(char c);

	std::list<Item*>* GetSubgroup(char c, int i);
	void PrintSubgroupByNames(char c, int i);
	void PrintSubgroupByDates(char c, int i);
	int CountSubgroupItems(char c, int i);

	Item* GetItem(char c, int i, std::string s);
	void PrintItem(char c, int i, std::string s);

	Item* InsertItem(std::string itemString); // overload to add items as string from pipe
	Item* InsertItem(char c, int i, std::string s, Date d);
	std::list<Item*>* InsertSubgroup(char s, int i, std::initializer_list<Item*> items);
	std::map<int, std::list<Item*>*>* InsertGroup(char c, std::initializer_list<int> subgroups, std::initializer_list<std::initializer_list<Item*>> items);

	bool RemoveItem(char c, int i, std::string s);
	bool RemoveSubgroup(char c, int i);
	bool RemoveGroup(char c);
};

