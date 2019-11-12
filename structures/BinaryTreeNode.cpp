//
// Created by hlefebvr on 12/11/19.
//

#include "BinaryTreeNode.h"

unsigned long int BinaryTreeNode::_node_id = 0;
unsigned long int BinaryTreeNode::get_next_node_id() { return _node_id++; }

unsigned long int BinaryTreeNode::id() const {
    return _id;
}