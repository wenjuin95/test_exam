#ifndef VECT2_HPP
#define VECT2_HPP

#include <vector>
#include <ostream>
#include <iostream>

class vect2
{
	private:
		std::vector<int> _con;
	public:
		vect2();
		vect2(int v1, int v2);
		vect2(const vect2 &src);
		vect2 &operator=(const vect2 &src);
		~vect2();

		int get_con(int arr) const;

		vect2 operator+(int v) const;
		vect2 operator+(const vect2 &src) const;
		vect2 &operator++(void);
		vect2 operator++(int);

		vect2 operator-(int v) const;
		vect2 operator-(const vect2 &src) const;
		vect2 &operator--(void);
		vect2 operator--(int);

		vect2 operator*(int v) const;
		vect2 operator*(const vect2 &src) const;

		bool operator==(const vect2 &src) const;
		bool operator!=(const vect2 &src) const;

		vect2 operator+=(int v);
		vect2 operator+=(const vect2 &src);
		vect2 operator-=(int v);
		vect2 operator-=(const vect2 &src);
		vect2 operator*=(int v);
		vect2 operator*=(const vect2 &src);

		int &operator[](int pos);
		int operator[](int pos) const;
};

vect2 operator*(int v, const vect2 &src);
std::ostream &operator<<(std::ostream &out, const vect2 &src);

#endif
