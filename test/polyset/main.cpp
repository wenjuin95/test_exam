#include "searchable_tree_bag.hpp"
#include "searchable_array_bag.hpp"
#include <cstdlib>
#include <iostream>
#include "set.hpp"


int main(int ac, char **av)
{
	if (ac == 1)
		return 1;
	SearchableBag *t = new searchable_tree_bag;
	SearchableBag *a = new searchable_array_bag;
	for (int i = 1; i < ac; i++)
	{
		t->insert(atoi(av[i]));
		a->insert(atoi(av[i]));
	}

	std::cout << "----- test:\n";
	t->print();
	a->print();

	for (int i = 1; i < ac; i++)
	{
		std::cout << "----- tree:\n";
		std::cout << atoi(av[i]) << ": " << (t->has(atoi(av[i])) == true ? "true" : "false") << std::endl;
		std::cout << atoi(av[i]) - 1 << ": " << (t->has(atoi(av[i]) - 1) == true ? "true" : "false")<< std::endl << std::endl;

		std::cout << "----- array:\n";
		std::cout << atoi(av[i]) << ": " << (a->has(atoi(av[i])) == true ? "true" : "false") << std::endl;
		std::cout << atoi(av[i]) - 1 << ": " << (a->has(atoi(av[i]) - 1) == true ? "true" : "false")<< std::endl << std::endl;
	}
	// t->clear();
	// a->clear();

	const searchable_array_bag tmp(static_cast<searchable_array_bag &>(*a));

	std::cout << "----- tmp:\n";
	tmp.print();

	std::cout << "----- tmp has 1:\n" << (tmp.has(1) == true ? "true" : "false" )<< std::endl;

	set sa(*a);
	set st(*a);
	for (int i = 1; i < ac; i++)
	{
		st.insert(atoi(av[i]));
		sa.insert(atoi(av[i]));
	}

	std::cout << "----- set a has 1:\n" << (sa.has(atoi(av[1])) == true ? "true" : "false") << std::endl;

	std::cout << "----- set a:\n";
	sa.print();

	std::cout << "----- set a (get_bag):\n";
	sa.get_bag().print();

	std::cout << "----- set t:\n";
	st.print();

	sa.clear();
	int arr[] = {1, 2, 3, 4};
	sa.insert(arr, sizeof(arr)/sizeof(arr[0]));

	std::cout << "----- set a (after clear and insert):\n";
	sa.print();

    std::cout << "test\n";
    std::cout << "test\n";
    std::cout << "test\n";
    std::cout << "test\n";
    std::cout << "test\n";
    std::cout << "test\n";
    
}

//c++ -I ../../42_exam_rank5/level_00/solution/PolySet ../../42_exam_rank5/level_00/solution/PolySet/ArrayBag.cpp ../../42_exam_rank5/level_00/solution/PolySet/TreeBag.cpp searchable_array_bag.cpp searchable_tree_bag.cpp set.cpp main.cpp
