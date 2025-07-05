#ifndef VECT2_HPP
#define VECT2_HPP

#include <iostream>
#include <vector>
#include <ostream> //?

class Vect2 {
    private:
        std::vector<int> _container;
    public:
        Vect2();
        Vect2(int v1, int v2);
        Vect2(const Vect2 &src);
        Vect2 operator=(const Vect2 &src);
        ~Vect2();

        int getContainer(int array) const;

        Vect2 operator+(int nb) const;
        Vect2 operator+(const Vect2 &other) const;
        Vect2 operator++(void) const;
        Vect2 operator++(int nb) const;

        Vect2 operator-(int nb) const;
        Vect2 operator-(const Vect2 &other) const;
        Vect2 operator--(void) const;
        Vect2 operator--(int nb) const;

        Vect2 operator*(int nb);
        Vect2 operator*(const Vect2 &other);

        bool operator==(const Vect2 &other) const;
        bool operator!=(const Vect2 &other) const;
        Vect2 operator+=(int nb);
        Vect2 operator+=(const Vect2 &other);
        Vect2 operator+=(int nb);
        Vect2 operator+=(const Vect2 &other);
        Vect2 operator-=(int nb);
        Vect2 operator-=(const Vect2 &other);
        Vect2 operator*=(int nb);
        Vect2 operator*=(const Vect2 &other);

};

std::ostream &operator<<(std::ostream &out, const Vect2 &src);

#endif