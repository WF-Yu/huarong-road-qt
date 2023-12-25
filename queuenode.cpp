#include "queuenode.h"

QueueNode::QueueNode()
{

}

QueueNode::~QueueNode()
{
    delete node;
}
bool QueueNode::ifUsed(Board _board){
    if (node->board->equal(_board)) return false; // used
    else if (next != nullptr)
        return next->ifUsed(_board);
    return true; // not used
}
