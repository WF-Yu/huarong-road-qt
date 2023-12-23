#ifndef TREENODE_H
#define TREENODE_H
#include "board.h"
#include "movement.h"
class TreeNode
{
public:
    TreeNode();
private:
    Board board;
    TreeNode* parent;
    Movement move; // movement from parent to itself
};

#endif // TREENODE_H