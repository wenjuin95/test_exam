#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : TreeBag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src) : TreeBag(src) {}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src)
{
    if (this != &src)
    {
        TreeBag::operator=(src);
    }
    return *this;
}

searchable_tree_bag::~searchable_tree_bag() {}

bool searchable_tree_bag::has(int value) const {
    Node *current = root;
    while (current)
    {
        if (current->value == value)
            return true;
        else if (current->value > value)
            current = current->left;
        else if (current->value < value)
            current = current->right;
    }
    return false;
}