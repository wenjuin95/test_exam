#include "vect2.hpp"

//clear && c++ main.cpp vect2.cpp vect2.hpp && valgrind --leak-check=full --show-leak-kinds=all ./a.out
int	main(void)
{
	vect2		v1;
	vect2		v2(1, 2);
	const vect2	v3(v2);
	vect2		v4 = v2;

	std::cout << "v2 = " << v2 << std::endl; // 1, 2
	v4++;
	std::cout << "v4++ = " << v4 << std::endl;// 2, 3
	std::cout << "++v4 = " << ++v4 << std::endl;// 3, 4
	v4--;
	std::cout << "v4-- = " << v4 << std::endl;// 2, 3
	std::cout << "v4-- = " << --v4 << std::endl;// 1, 2

	std::cout << "[Personal Test] v1 == v1-> " << (v1 == v1) << std::endl; //1
	std::cout << "[Personal Test] v1 != v2-> " << (v1 != v2) << std::endl; //1
	std::cout << "[Personal Test] v1 != v1-> " << (v1 != v1) << std::endl; //0

	v2 += v3;
	std::cout << "v2 += v3-> " << v2 << std::endl;// 2, 4

	(v1 -= v2);
	std::cout << "v1 -= v2-> " << v1 << std::endl;// -2, -4

	v2 = v3 + v3;
	std::cout << "v2 = v3 + v3-> " << v2 << std::endl;// 2, 4

	v2 = 3 * v2;
	std::cout << "v2 = 3 * v2-> " << v2 << std::endl;// 6, 12

	v2 += v2 += v3;
	std::cout << "v2 += v2 += v3> " << v2 << std::endl;// 14, 28

	std::cout << "v1 == v3-> " << (v1 == v3) << std::endl; //0
	std::cout << "v1 == v1-> " << (v1 == v1) << std::endl; //1
	std::cout << "v1 != v3-> " << (v1 != v3) << std::endl; //1
}
