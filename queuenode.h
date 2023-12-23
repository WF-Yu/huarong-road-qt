#ifndef QUEUENODE_H
#define QUEUENODE_H
#include "treenode.h"

class QueueNode
{
public:
    QueueNode();
private:
    TreeNode node;
    QueueNode* next;
};

#endif // QUEUENODE_H
