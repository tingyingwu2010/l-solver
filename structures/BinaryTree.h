//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_BINARYTREE_H
#define ED_SOLVER_BINARYTREE_H

#include <functional>
#include <stack>
#include <fstream>
#include "../utils/Exception.h"
#include "BinaryTreeNode.h"

template<class Node>
class BinaryTree {
protected:
    BinaryTree<Node>* _left = nullptr;
    BinaryTree<Node>* _right = nullptr;
    Node _node;
    bool _unsafe_mode = false;

    // Transforming (specialized) traversal algorithms
    void depth_first_transform_pre_order(const std::function<bool(BinaryTree<Node>&)>& treat_node);
    void depth_first_transform_in_order(const std::function<bool(BinaryTree<Node>&)>& treat_node);
    void depth_first_transform_post_order(const std::function<bool(BinaryTree<Node>&)>& treat_node);

    // Const (specialized) traversal algorithms
    void depth_first_search_pre_order(const std::function<void(const BinaryTree<Node>&)>& treat_node) const;
    void depth_first_search_in_order(const std::function<void(const BinaryTree<Node>&)>& treat_node) const;
    void depth_first_search_post_order(const std::function<void(const BinaryTree<Node>&)>& treat_node) const;
public:
    enum TraversalOrder { InOrder, PreOrder, PostOrder };
    enum NodeSide {Left, Right};

    // Constructors
    BinaryTree() {}
    explicit BinaryTree(const Node& node);
    explicit BinaryTree(const BinaryTree<Node>& rhs);
    BinaryTree& operator=(const BinaryTree<Node>& rhs);
    virtual ~BinaryTree();

    // Getters
    virtual BinaryTree<Node>& child(NodeSide side);
    virtual Node& node();
    virtual bool unsafe_mode() const { return _unsafe_mode; }

    // Const getters
    virtual const BinaryTree<Node>& child(NodeSide side) const;
    virtual const Node& node() const;
    unsigned long int id() const;
    void unsafe_mode(bool unsafe_mode) { _unsafe_mode = unsafe_mode; }

    // Checkers
    virtual bool has_child(NodeSide side) const;

    // Setters
    virtual void insert(NodeSide where, BinaryTree<Node>& to_be_inserted);
    virtual void attach(NodeSide where, BinaryTree<Node>& to_be_attached);
    virtual void detach(NodeSide where);
    virtual void reduce_to(NodeSide where);
    virtual void node(const Node& node);

    // Swap and transfers
    static void transfer(NodeSide from_side, BinaryTree<Node>& from_src, NodeSide to_side, BinaryTree<Node>& to_src);
    static void swap(NodeSide side1, BinaryTree<Node>& src1, NodeSide side2, BinaryTree<Node>& src2);

    // setter by copy / delete
    virtual void insert_copy(NodeSide where, const BinaryTree<Node>& to_be_inserted);
    virtual void attach_copy(NodeSide where, const BinaryTree<Node>& to_be_attached);
    virtual void detach_delete(NodeSide where);
    virtual void reduce_to_delete(NodeSide where);

    // Transforming traversal algorithms
    void depth_first_transform(TraversalOrder order, const std::function<bool(BinaryTree<Node>&)>& treat_node);
    void breadth_first_transform(const std::function<void(BinaryTree<Node>&)>& treat_node);

    // Const traversal algorithms
    void depth_first_search(TraversalOrder order, const std::function<void(const BinaryTree<Node>&)>& treat_node) const;
    void breadth_first_search(const std::function<bool(const BinaryTree<Node>&)>& treat_node) const;

    // Display and output-related functions
    std::string to_string() const;
    void export_to_dot(const std::string& filename = "binary_tree", bool with_system_command = false) const;
};

template<class Node>
static std::ostream& operator<<(std::ostream& os, const BinaryTree<Node>& node);

#include "BinaryTree.cpp"

#endif //ED_SOLVER_BINARYTREE_H
