#ifndef SET_HPP
#define SET_HPP

// #include "SearchableBag.hpp"
#include "../../42_exam_rank5/level_00/solution/PolySet/SearchableBag.hpp"

class set {
	private:
		SearchableBag *bag;

	public:
		set(SearchableBag &bag);
		set (const set &src);
		set &operator=(const set &src);
		~set();

		SearchableBag &get_bag() const;
		void insert(int value);
		void insert(int *arr, int size);
		bool has(int value) const;
		void print() const;
		void clear();
};

#endif
