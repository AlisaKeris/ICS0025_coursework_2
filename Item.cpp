#include "Item.h"
#include "RandomUtils.h"

#include <random>
#include <string.h>
#include <iostream>

Item::Item() 
{
	std::default_random_engine generator;
	RandomUtils random(generator);

	Group = random.getRandomGroup();
	Subgroup = random.getRandomSubgroup();
	Name = random.getRandomName();
	Timestamp = random.getRandomTimestamp();
}

Item::Item(const Item& Original)
{ 
	Group = Original.Group;
	Subgroup = Original.Subgroup;
	Timestamp = Original.Timestamp;
	Name.assign(Original.Name);
}

Item::~Item()
{
	std::cout << "destroying Item \"" << Name << '\"'<<std::endl;
}

Item::Item(char group, int subgroup, std::string name, Date date) 
{
	Group = group;
	Subgroup = subgroup;
	Name.assign(name);
	Timestamp = Date(date);
}

std::string Item::ToString() const 
{
	return Name + " " + Timestamp.ToString();
}

int Item::getSubgroup() const 
{
	return Subgroup;
}

char Item::getGroup() const
{
	return Group;
}

std::string Item::getName() const
{
	return Name;
}

Date Item::getTimestamp() const
{
	return Timestamp;
}
