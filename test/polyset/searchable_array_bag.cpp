#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag() : ArrayBag() {}

searchable_array_bag::searchable_array_bag(const searchable_array_bag &src) : ArrayBag(src) {}

searchable_array_bag &searchable_array_bag ::operator=(const searchable_array_bag &src)
{
	if (this != &src)
	{
		ArrayBag::operator=(src);
	}
	return *this;
}

searchable_array_bag::~searchable_array_bag()
{

}

bool searchable_array_bag::has(int value) const
{
	for (int i = 0; i < this->_size; i++)
	{
		if (this->_data[i] == value)
			return true;
	}
	return false;
}
