#ifndef TREENODE_H
#define TREENODE_H
#include "board.h"
class TreeNode
{
public:
    TreeNode();
    ~TreeNode();
    Board* board;
    TreeNode* parent;
    Movement* move; // movement from parent to itself
};

#endif // TREENODE_H
