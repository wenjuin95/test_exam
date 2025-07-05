#include "vect2.hpp"

Vect2::Vect2()
{
    this->_container.push_back(0);
    this->_container.push_back(0);
}

Vect2::Vect2(int v1, int v2)
{
    this->_container.push_back(v1);
    this->_container.push_back(v2);
}

Vect2::Vect2(const Vect2 &src)
{
    *this = src;
}

Vect2 Vect2::operator=(const Vect2 &src)
{
    if (this != &src)
    {
        this->_container = src;
    }
}