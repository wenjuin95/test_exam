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

searchable_tree_bag::~searchable_tree_bag(){}

bool searchable_tree_bag::has(int value) const
{
	Node *curr = root;
	while (curr)
	{
		if (curr->value == value)
			return true;
		else if (curr->value < value)
			curr = curr->right;
		else
			curr = curr->left;
	}
	return false;
}
