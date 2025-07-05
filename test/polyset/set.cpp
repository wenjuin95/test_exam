#include "set.hpp"

set::set(SearchableBag &bag) : bag(&bag) {}

set::set(const set &src) : bag(src.bag) {}

set &set::operator=(const set &src)
{
	if (this != &src)
	{
		this->bag = src.bag;
	}
	return *this;
}

set::~set() {}

SearchableBag &set::get_bag() const
{
	return *this->bag;
}

void set::insert(int value)
{
	if (!this->bag->has(value))
		this->bag->insert(value);
}

void set::insert(int *arr, int size)
{
	if (arr == NULL || size <= 0)
		return ;
	for (int i = 0; i < size; i++)
	{
		if (!this->bag->has(arr[i]))
			this->bag->insert(arr[i]);
	}
}

bool set::has(int value) const
{
	return this->bag->has(value);
}

void set::print() const
{
	bag->print();
}

void set::clear()
{
	bag->clear();
}
