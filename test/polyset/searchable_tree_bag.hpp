#ifndef SEARCHABLE_TREE_BAG
#define SEARCHABLE_TREE_BAG

//#include "TreeBag.hpp"
#include "../../42_exam_rank5/level_00/solution/PolySet/TreeBag.hpp"
//#include "SearchableBag.hpp"
#include "../../42_exam_rank5/level_00/solution/PolySet/SearchableBag.hpp"

class searchable_tree_bag : public TreeBag, public SearchableBag {
    public:
        searchable_tree_bag();
        searchable_tree_bag(const searchable_tree_bag &src);
        searchable_tree_bag &operator=(const searchable_tree_bag &src);
        ~searchable_tree_bag();

        virtual bool has(int value) const;
};

#endif