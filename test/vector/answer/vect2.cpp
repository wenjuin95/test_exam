#include "vect2.hpp"

vect2::vect2()
{
    this->_container.push_back(0);
    this->_container.push_back(0);
}

vect2::vect2(int v1, int v2)
{
    this->_container.push_back(v1);
    this->_container.push_back(v2);
}

vect2::vect2(const vect2 &src)
{
    *this = src;
}

vect2 &vect2::operator=(const vect2 &src)
{
    if (this != &src)
    {
        this->_container = src._container;
    }
	return *this;
}

vect2::~vect2() {}

int vect2::getContainer(int array) const
{
	return this->_container[array];
}

vect2 vect2::operator+(int value) const
{
	vect2 copy(*this);
	copy._container[0] += value;
	copy._container[1] += value;
	return copy;
}

vect2 vect2::operator+(const vect2 &other) const
{
	vect2 copy(*this);
	copy._container[0] += other._container[0];
	copy._container[1] += other._container[1];
	return copy;
}

vect2 &vect2::operator++(void)
{
	this->_container[0] += 1;
	this->_container[1] += 1;
	return *this;
}

vect2 vect2::operator++(int)
{
	vect2 copy(*this);
	operator++();
	return copy;
}


vect2 vect2::operator-(int value) const
{
	vect2 copy(*this);
	copy._container[0] -= value;
	copy._container[1] -= value;
	return copy;
}

vect2 vect2::operator-(const vect2 &other) const
{
	vect2 copy(*this);
	copy._container[0] -= other._container[0];
	copy._container[1] -= other._container[1];
	return copy;
}

vect2 &vect2::operator--(void)
{
	this->_container[0] -= 1;
	this->_container[1] -= 1;
	return *this;
}

vect2 vect2::operator--(int)
{
	vect2 copy(*this);
	operator--();
	return copy;
}

vect2 vect2::operator*(int nb) const
{
	vect2 copy(*this);
	copy._container[0] *= nb;
	copy._container[1] *= nb;
	return copy;
}

vect2 vect2::operator*(const vect2 &other) const
{
	vect2 copy(*this);
	copy._container[0] = other._container[0];
	copy._container[1] = other._container[1];
	return copy;
}


bool vect2::operator==(const vect2 &other) const
{
	if (this->_container[0] == other._container[0] && this->_container[1] == other._container[1])
		return true;
	return false;
}

bool vect2::operator!=(const vect2 &other) const
{
	if (this->_container[0] != other._container[0] && this->_container[1] != other._container[1])
		return true;
	return false;
}

vect2 vect2::operator+=(int nb)
{
	this->_container[0] += nb;
	this->_container[1] += nb;
	return *this;
}

vect2 vect2::operator+=(const vect2 &other)
{
	this->_container[0] += other._container[0];
	this->_container[1] += other._container[1];
	return *this;
}

vect2 vect2::operator-=(int nb)
{
	this->_container[0] -= nb;
	this->_container[1] -= nb;
	return *this;
}

vect2 vect2::operator-=(const vect2 &other)
{
	this->_container[0] -= other._container[0];
	this->_container[1] -= other._container[1];
	return *this;
}

vect2 vect2::operator*=(int nb)
{
	this->_container[0] *= nb;
	this->_container[1] *= nb;
	return *this;
}

vect2 vect2::operator*=(const vect2 &other)
{
	this->_container[0] *= other._container[0];
	this->_container[1] *= other._container[1];
	return *this;
}

// the & is used to return a reference to the element at the specified position
// this allows for modification of the element directly through the operator
// if no "&" is used, it would return a copy of the element so it could not be modified
int &vect2::operator[](int pos)
{
	return (pos == 0) ? this->_container[0] : this->_container[1];
}

int vect2::operator[](int pos) const
{
	return (pos == 0) ? this->_container[0] : this->_container[1];
}

vect2 operator*(int v, const vect2 &src)
{
	return (src * v);
}

vect2 operator*(int value, const vect2 &other)
{
	return (other * value);
}

std::ostream &operator<<(std::ostream &out, const vect2 &src)
{
	out << "{" << src.getContainer(0) << ", " << src.getContainer(1) << "}" << std::endl;
	return out;
}
