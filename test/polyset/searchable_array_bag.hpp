#ifndef SEARCHABLE_ARRAY_BAG
#define SEARCHABLE_ARRAY_BAG

//#include "ArrayBag.hpp"
#include "../../42_exam_rank5/level_00/solution/PolySet/ArrayBag.hpp"
//#include "SearchableBag.hpp"
#include "../../42_exam_rank5/level_00/solution/PolySet/SearchableBag.hpp"

class searchable_array_bag : public ArrayBag, public SearchableBag {
    public:
        searchable_array_bag();
        searchable_array_bag(const searchable_array_bag &src);
        searchable_array_bag &operator=(const searchable_array_bag &src);
        ~searchable_array_bag();

        virtual bool has(int value) const;
};


#endif