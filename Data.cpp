#include "Data.h"
#include "Item.h"
#include "RandomUtils.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>

Data::Data()
{
}

Data::Data(int n)
{
	std::default_random_engine generator;
	RandomUtils random(generator);

	for (int i = 0; i < n; i++) {
		char group = random.getRandomGroup();
		int subgroup = random.getRandomSubgroup();
		std::string name = random.getRandomName();
		Date d = random.getRandomTimestamp();

		InsertItem(group, subgroup, name, d);
	}
}

Data::~Data()
{

	for (auto& outerMapIt : DataStructure) {

		for (auto& innerMapIt : *outerMapIt.second) {
			for (auto& it3 : *innerMapIt.second) {
				// deallocate every element of the list
				delete it3;
			}
			// deallocate the list
			std::cout << "destroying Subgroup \"" << innerMapIt.first << '\"' << std::endl;
			delete innerMapIt.second;
		}
		// deallocate the inner map
		std::cout << "destroying Group \"" << outerMapIt.first << '\"' << std::endl;
		delete outerMapIt.second;
	}

	// TODO map itself is in stack, is this necessary?  
	DataStructure.clear();
}

Item* Data::InsertItem(std::string itemString)
{
	char _groupName; 
	int _subgroup; 
	std::string _name; 
	Date _date;

	// Parse the string received from server
    std::string mon;
	int day,year;
	
	std::stringstream ss(itemString);
	ss >> _groupName >> _subgroup;

	size_t nameDelimiterStartPos = itemString.find('<') + 1;
	size_t nameDelimiterEndPos = itemString.find('>') - (itemString.find('<') + 1);
	_name = itemString.substr(nameDelimiterStartPos, nameDelimiterEndPos);

	size_t dateStartPos = itemString.find('>') + 1;
	size_t dateEndPos = itemString.length() - (itemString.find('>') + 1);
	std::stringstream dateSS(itemString.substr(dateStartPos, dateEndPos));
	dateSS >> day >> mon >> year;

	// find number of month by name
	int monthNum=1;
	_date = Date(day, monthNum, year);

	// Perform insert normally
	return InsertItem(_groupName, _subgroup, _name, _date);
}

Item* Data::InsertItem(char c, int i, std::string s, Date d)
{
	try	{
		auto it = DataStructure.find(c);

		Item* newItem = new Item(c, i, s, d);

		Item* existing = GetItem(c, i, s);
		if (existing) { // item already exists
			return nullptr;
		}

		if (it != DataStructure.end()) { // group exists
			auto group = it->second;
			auto subgroupIt = group->find(i);

			if (subgroupIt != group->end()) { // subgroup exists
				subgroupIt->second->push_back(newItem);
			}
			else
			{
				InsertSubgroup(c, i, { newItem });
			}
		}
		else {
			InsertGroup(c, { i }, { { newItem } });
		}

		return newItem;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return nullptr;
	}
}

std::list<Item*>* Data::InsertSubgroup(char s, int i, std::initializer_list<Item*> items)
{
	try {
		auto existingSubgroup = GetSubgroup(s, i);
		if (existingSubgroup) {
			return nullptr;
		}

		auto groupIt = DataStructure.find(s);

		if (groupIt != DataStructure.end()) { // group exists
			groupIt->second->emplace(i, new std::list<Item*>(items));
			return groupIt->second->find(i)->second;
		}
		else {
			InsertGroup(s, { i }, { items });
			groupIt = DataStructure.find(s); // update to existing group
			return groupIt->second->find(i)->second;
		}

		return nullptr;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return nullptr;
	}
	
}

std::map<int, std::list<Item*>*>* Data::InsertGroup(char c, std::initializer_list<int> subgroupKeys, std::initializer_list<std::initializer_list<Item*>> items)
{
	try {
		auto existingGroup = GetGroup(c);
		if (existingGroup) {
			return nullptr;
		}

		std::map<int, std::list<Item*>*>* group = new std::map<int, std::list<Item*>*>();

		for (int sg : subgroupKeys) {
			std::list<Item*>* newSubgroup = new std::list<Item*>(); // empty subgroup
			for (auto itemList : items) {
				for (Item* item : itemList) {
					if (item->getSubgroup() == sg) {
						newSubgroup->push_back(item); // fill with corresponding items
					}
				}
			}
			group->emplace(sg, newSubgroup);  // insert subgroup
		}
		DataStructure.emplace(c, group);  // insert group
		return group;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return nullptr;
	}

}

void Data::PrintAll()
{
	auto print = [](const Item* item) { std::cout << "  - " << item->ToString() << std::endl; };

	for (auto it = DataStructure.cbegin(); it != DataStructure.cend(); ++it) {
		auto group = it->second;
		std::cout << it->first << ':' << std::endl; // print group key

		for (auto inner_it = group->cbegin(); inner_it != group->cend(); ++inner_it) {
			std::cout << " " << inner_it->first << ':' << std::endl; // print subgroup key

			// print items under subgroup
			std::for_each(inner_it->second->cbegin(), inner_it->second->cend(), print);
		}

		std::cout << std::endl << std::endl;
	}
}

int Data::CountItems()
{
	int totalSize = 0;
	for (auto it = DataStructure.cbegin(); it != DataStructure.cend(); ++it) {
		auto subgroup = it->second;
		for (auto inner_it = subgroup->cbegin(); inner_it != subgroup->cend(); ++inner_it) {
			totalSize += inner_it->second->size();
		}
	}

	return totalSize;
}

std::map<int, std::list<Item*>*>* Data::GetGroup(char c)
{
	auto groupIt = DataStructure.find(c);
	if (groupIt != DataStructure.end()) { // group exists
		return groupIt->second;
	}

	return nullptr;
}

void Data::PrintGroup(char c)
{
	auto group = GetGroup(c);
	if (!group) {
		throw std::invalid_argument("Group not found!\n");
	}

	auto print = [](const Item* item) { std::cout << "  - " << item->ToString() << std::endl; };

	std::cout << c << ':' << std::endl; // print group

	for (auto inner_it = group->cbegin(); inner_it != group->cend(); ++inner_it) {
		std::cout << " " << inner_it->first << ':' << std::endl; // print subgroup

		// print items under subgroup
		std::for_each(inner_it->second->cbegin(), inner_it->second->cend(), print);
	}

	std::cout << std::endl << std::endl; // print group 
}

int Data::CountGroupItems(char c)
{
	auto group = GetGroup(c);
	if (!group) {
		return 0;
	}

	int totalSize = 0;
	for (auto inner_it = group->cbegin(); inner_it != group->cend(); ++inner_it) {
		totalSize += inner_it->second->size();
	}

	return totalSize;
}


std::list<Item*>* Data::GetSubgroup(char c, int i) {

	auto group = GetGroup(c);
	if (!group) {
		return nullptr;
	}

	auto subgroupIt = group->find(i);
	if (subgroupIt != group->end()) { // subgroup exists
		return subgroupIt->second;
	}

	return nullptr;
}

void Data::PrintSubgroupByNames(char c, int i) {
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) {
		throw std::invalid_argument("Subgroup not found!\n");
	}

	subgroup->sort([](Item* lhs, Item* rhs) {return lhs->getName() < rhs->getName(); });

	std::cout << std::endl << "** Subgroup sorted by names **" << std::endl << c << ':' << std::endl << " " << i << ":" << std::endl; // print group

	auto print = [](const Item* item) { std::cout << "  - " << item->ToString() << std::endl; };
	std::for_each(subgroup->cbegin(), subgroup->cend(), print);
}

void Data::PrintSubgroupByDates(char c, int i) {
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) {
		throw std::invalid_argument("Subgroup not found!\n");
	}

	subgroup->sort([](Item* lhs, Item* rhs) {return lhs->getTimestamp() < rhs->getTimestamp(); });

	std::cout << std::endl << "** Subgroup sorted by dates **" << std::endl << c << ':' << std::endl << " " << i << ":" << std::endl; // print group

	auto print = [](const Item* item) { std::cout << "  - " << item->ToString() << std::endl; };
	std::for_each(subgroup->cbegin(), subgroup->cend(), print);
}

int Data::CountSubgroupItems(char c, int i) {
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) {
		return 0;
	}

	return subgroup->size();
}

Item* Data::GetItem(char c, int i, std::string s) {
	auto subgroup = GetSubgroup(c, i);
	if (!subgroup) {
		return nullptr;
	}

	Item testItem = Item(c, i, s, Date());
	auto itemIter = std::find_if(subgroup->begin(), subgroup->end(),
		[&testItem](const Item* x) { return (x->getName() == testItem.getName() && x->getSubgroup() == testItem.getSubgroup() && x->getGroup() && testItem.getGroup()); });

	if (itemIter != subgroup->end()) { // item exists
		return *itemIter;
	}

	return nullptr;
}

void Data::PrintItem(char c, int i, std::string s) {
	Item* item = GetItem(c, i, s);

	if (!item) {
		throw std::invalid_argument("Item not found!\n");
	}

	std::cout << item->ToString() << std::endl;
}


bool Data::RemoveItem(char c, int i, std::string s)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);
	auto item = GetItem(c, i, s);

	if (group && subgroup && item) {

		Item testItem = Item(c, i, s, Date());
		auto iterator = std::remove_if(subgroup->begin(), subgroup->end(),
			[&testItem](const Item* x) { return (x->getName() == testItem.getName() && x->getSubgroup()==testItem.getSubgroup() && x->getGroup() && testItem.getGroup()); }
		);

		subgroup->erase(iterator, subgroup->end());

		// if after removing item subgroup is empty,
		// remove subgroup too
		if (subgroup->size() == 0) {
			return RemoveSubgroup(c, i);
		}

		return true;
	}

	return false;
}

bool Data::RemoveSubgroup(char c, int i)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);

	if (group && subgroup) {
		for (auto& item : *subgroup) {
			delete item;
		}
		delete subgroup;

		group->erase(i);

		// if after removing subgroup, group is empty,
		// remove group too
		if (group->size() == 0) {
			return RemoveGroup(c);
		}

		return true;
	}

	return false;
}

bool Data::RemoveGroup(char c)
{
	auto group = GetGroup(c);
	if (group) {
		for (auto innerMapIt = group->cbegin(); innerMapIt != group->cend(); ++innerMapIt) {
			auto list = innerMapIt->second;
			for (auto& item : *list) {
				delete item;
			}
			delete list;
		}

		delete group;
	}
	
	return DataStructure.erase(c);
}
