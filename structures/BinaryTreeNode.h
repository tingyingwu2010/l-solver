//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_BINARYTREENODE_H
#define ED_SOLVER_BINARYTREENODE_H

#include <string>

class BinaryTreeNode {
    static unsigned long int _node_id;
    static unsigned long int get_next_node_id();
    unsigned long int _id = get_next_node_id();
    bool _color = false;
public:
    BinaryTreeNode() = default;
    BinaryTreeNode(const BinaryTreeNode& rhs) = default;
    BinaryTreeNode& operator=(const BinaryTreeNode& rhs) = default;
    virtual ~BinaryTreeNode() = default;
    unsigned long int id() const;
    virtual std::string to_string() const = 0;
    void color(bool color) { _color = color; }
    bool color() const { return _color; }
};


#endif //ED_SOLVER_BINARYTREENODE_H
