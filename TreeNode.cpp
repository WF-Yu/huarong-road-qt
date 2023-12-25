#include "treenode.h"

TreeNode::TreeNode()
{

}
TreeNode::~TreeNode()
{
    delete board;
    delete move;
}
