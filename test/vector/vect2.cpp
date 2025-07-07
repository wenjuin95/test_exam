#include "vect2.hpp"

vect2::vect2()
{
	this->_con.push_back(0);
	this->_con.push_back(0);
}

vect2::vect2(int v1, int v2)
{
	this->_con.push_back(v1);
	this->_con.push_back(v2);
}

vect2::vect2(const vect2 &src)
{
	this->_con = src._con;
}

vect2 &vect2::operator=(const vect2 &src)
{
	if (this != &src)
	{
		this->_con = src._con;
	}
	return *this;
}

vect2::~vect2() {}

int vect2::get_con(int arr) const
{
	return this->_con[arr];
}

vect2 vect2::operator+(int v) const
{
	vect2 copy(*this);
	copy._con[0] += v;
	copy._con[1] += v;
	return copy;
}

vect2 vect2::operator+(const vect2 &src) const
{
	vect2 copy(*this);
	copy._con[0] += src._con[0];
	copy._con[1] += src._con[1];
	return copy;
}

vect2 &vect2::operator++(void)
{
	this->_con[0] += 1;
	this->_con[1] += 1;
	return *this;
}

vect2 vect2::operator++(int)
{
	vect2 copy(*this);
	operator++();
	return copy;
}

vect2 vect2::operator-(int v) const
{
	vect2 copy(*this);
	copy._con[0] -= v;
	copy._con[1] -= v;
	return copy;
}

vect2 vect2::operator-(const vect2 &src) const
{
	vect2 copy(*this);
	copy._con[0] -= src._con[0];
	copy._con[1] -= src._con[1];
	return copy;
}

vect2 &vect2::operator--(void)
{
	this->_con[0] -= 1;
	this->_con[1] -= 1;
	return *this;
}

vect2 vect2::operator--(int)
{
	vect2 copy(*this);
	operator--();
	return copy;
}

vect2 vect2::operator*(int v) const
{
	vect2 copy(*this);
	copy._con[0] *= v;
	copy._con[1] *= v;
	return copy;
}

vect2 vect2::operator*(const vect2 &src) const
{
	vect2 copy(*this);
	copy._con[0] *= src._con[0];
	copy._con[1] *= src._con[1];
	return copy;
}

bool vect2::operator==(const vect2 &src) const
{
	if (this->_con[0] == src._con[0] && this->_con[1] == src._con[1])
		return true;
	return false;
}

bool vect2::operator!=(const vect2 &src) const
{
	if (this->_con[0] != src._con[0] && this->_con[1] != src._con[1])
		return true;
	return false;
}

vect2 vect2::operator+=(int v)
{
	this->_con[0] += v;
	this->_con[1] += v;
	return *this;
}

vect2 vect2::operator+=(const vect2 &src)
{
	this->_con[0] += src._con[0];
	this->_con[1] += src._con[1];
	return *this;
}

vect2 vect2::operator-=(int v)
{
	this->_con[0] -= v;
	this->_con[1] -= v;
	return *this;
}

vect2 vect2::operator-=(const vect2 &src)
{
	this->_con[0] -= src._con[0];
	this->_con[1] -= src._con[1];
	return *this;
}

vect2 vect2::operator*=(int v)
{
	this->_con[0] *= v;
	this->_con[1] *= v;
	return *this;
}

vect2 vect2::operator*=(const vect2 &src)
{
	this->_con[0] *= src._con[0];
	this->_con[1] *= src._con[1];
	return *this;
}

// the & is used to return a reference to the element at the specified position
// this allows for modification of the element directly through the operator
// if no "&" is used, it would return a copy of the element so it could not be modified
int &vect2::operator[](int pos)
{
	return (pos == 0) ? this->_con[0] : this->_con[1];
}

int vect2::operator[](int pos) const
{
	return (pos == 0) ? this->_con[0] : this->_con[1];
}

vect2 operator*(int v, const vect2 &src)
{
	return (src * v);
}


std::ostream &operator<<(std::ostream& out, const vect2 &src)
{
	out << "{" << src.get_con(0) << ", " << src.get_con(1) << "}";
	return out;
}
