#ifndef QUEUENODE_H
#define QUEUENODE_H
#include "treenode.h"

class QueueNode
{
public:
    QueueNode();
    ~QueueNode();

    bool ifUsed(Board); // return true if not used
    TreeNode* node;
    QueueNode* next;
};

#endif // QUEUENODE_H
