#ifndef VECT2_HPP
#define VECT2_HPP

#include <iostream>
#include <vector>
#include <ostream>

class vect2 {
    private:
        std::vector<int> _container;
    public:
        vect2();
        vect2(int v1, int v2);
        vect2(const vect2 &src);
        vect2 &operator=(const vect2 &src);
        ~vect2();

        int getContainer(int array) const;

        vect2 operator+(int nb) const;
        vect2 operator+(const vect2 &other) const;
        vect2 &operator++(void);
        vect2 operator++(int);

        vect2 operator-(int nb) const;
        vect2 operator-(const vect2 &other) const;
        vect2 &operator--(void);
        vect2 operator--(int);

        vect2 operator*(int nb) const;
        vect2 operator*(const vect2 &other) const;

        bool operator==(const vect2 &other) const;
        bool operator!=(const vect2 &other) const;
        vect2 operator+=(int nb);
        vect2 operator+=(const vect2 &other);
        vect2 operator-=(int nb);
        vect2 operator-=(const vect2 &other);
        vect2 operator*=(int nb);
        vect2 operator*=(const vect2 &other);

		int &operator[](int pos);
		int operator[](int pos) const;
};

vect2 operator*(int value, const vect2 &other);
std::ostream &operator<<(std::ostream &out, const vect2 &src);

#endif
