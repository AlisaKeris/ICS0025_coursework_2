#pragma once

#include "Date.h"
#include <string>

class Item
{
private:
	char Group;    // Any from range 'A'...'Z'
	int Subgroup;   // Any from range 0...99
	std::string Name;    // Any, but not empty 
	Date Timestamp; // Any 

public:
	Item();         // Fills the four fields above with pseudo-random values
	Item(char, int, std::string, Date);
	Item(const Item&); // copy constructor
	~Item();
	std::string ToString() const;
	int getSubgroup() const;
	char getGroup() const;
	std::string getName() const;
	Date getTimestamp() const;
};

