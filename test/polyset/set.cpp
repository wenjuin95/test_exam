#include "set.hpp"
#include <iostream>

set::set(SearchableBag &bag) : _bag(&bag) {}

set::set(const set &src) : _bag(src._bag) {}

set &set::operator=(const set &src)
{
    if (this != &src)
    {
        this->_bag = src._bag;
    }
    return *this;
}

set::~set() {}

SearchableBag &set::get_bag() const
{
    return *this->_bag;
}

void set::insert(int value)
{
    if (this->_bag->has(value) == true)
        this->_bag->insert(value);
}

void set::insert(int *arr_value, int size)
{
    if (arr_value == NULL || size <= 0)
        return ;
    for (int i = 0; i < size; i++)
    {
        if (this->_bag->has(arr_value[i]) == true)
            this->_bag->insert(arr_value[i]);
    }
}

bool set::has(int value) const
{
    return this->_bag->has(value);
}

void set::print() const
{
    this->_bag->print();
}

void set::clear()
{
    this->_bag->clear();
}