#include <string.h>

#include "bynar_tree.h"

ErrorBynarTree create_bynar_top(const void* value, size_t size_node, Node** node)
{
    if (node == NULL || value == NULL)
        return BYNAR_ERROR_NULL_PTR;

    *node = (Node*)calloc(1, sizeof(Node) + size_node);
    if (*node == NULL)
        return BYNAR_ERROR_ALLOCATION;

    (*node)->elem = (char*)(*node) + sizeof(Node);
    memcpy((*node)->elem, value, size_node);

    return BYNAR_ERROR_MO;
}

ErrorBynarTree create_bynar_tree(BynarTree* tree, size_t size_elem)
{
    if (tree == NULL)
        return BYNAR_ERROR_NULL_PTR;

    tree->size_elem = size_elem;
    return BYNAR_ERROR_MO;
}

ErrorBynarTree NewNode(TypeKeyWords type, int cnst, Node** node)
{
    Token temp_token = {};
    temp_token.type_key_word = type;

    if (type == CONST)
        temp_token.cnst = cnst;

    return create_bynar_top(&temp_token, sizeof(Token), node);
}