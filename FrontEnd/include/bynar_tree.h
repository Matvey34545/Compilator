#ifndef BYNAR_TREE
#define BYNAR_TREE

#include <stdlib.h>
#include "syntax.h"

enum ErrorBynarTree
{
    BYNAR_ERROR_MO         = 0,
    BYNAR_ERROR_ALLOCATION = 1,
    BYNAR_ERROR_NULL_PTR   = 2
};

struct Node
{
    void* elem;
    Node* left;
    Node* right;
};

struct BynarTree
{
    Node* root;
    size_t size_elem;
};

ErrorBynarTree create_bynar_top(const void* value, size_t size_node, Node** node);
ErrorBynarTree create_bynar_tree(BynarTree* tree, size_t size_elem);
ErrorBynarTree NewNode(TypeKeyWords type, int cnst, Node** node);

#endif