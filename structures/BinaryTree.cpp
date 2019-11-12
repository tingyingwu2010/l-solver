//
// Created by hlefebvr on 12/11/19.
//

#include <iostream>


template <class Node>
L::BinaryTree<Node>::BinaryTree(const Node& node) : _node(node) {}

template<class Node>
L::BinaryTree<Node>::BinaryTree(const BinaryTree<Node>& rhs) :
        _node(rhs._node),
        _right(rhs.has_child(Right) ? new BinaryTree<Node>(*rhs._right) : nullptr),
        _left(rhs.has_child(Left) ? new BinaryTree<Node>(*rhs._left) : nullptr)
{}


template <class Node>
L::BinaryTree<Node>::~BinaryTree() {
    if (!_unsafe_mode) {
        delete _left;
        _left = nullptr;
        delete _right;
        _right = nullptr;
    }
}

template <class Node>
L::BinaryTree<Node>& L::BinaryTree<Node>::operator=(const BinaryTree<Node>& rhs) {
    delete _left;
    delete _right;
    _left = nullptr;
    _right = nullptr;
    _node = rhs.node();
    if (rhs.has_child(Left)) attach_copy(Left, rhs.child(Left));
    if (rhs.has_child(Right)) attach_copy(Right, rhs.child(Right));
    return *this;
}

template<class Node>
unsigned long int L::BinaryTree<Node>::id() const {
    return _node.id();
}

template<class Node>
bool L::BinaryTree<Node>::has_child(NodeSide side) const {
    if (side != Left && side != Right) throw Exception("Unknown child side given");
    return (side == Left) ? (_left != nullptr) : (_right != nullptr);
}

template<class Node>
L::BinaryTree<Node> &L::BinaryTree<Node>::child(NodeSide side) {
    if (!has_child(side)) throw Exception("Trying to access non existing child");
    return (side == Left) ? *_left : *_right;
}

template<class Node>
const L::BinaryTree<Node> &L::BinaryTree<Node>::child(NodeSide side) const {
    if (!has_child(side)) throw Exception("Trying to access non existing child");
    return (side == Left) ? *_left : *_right;
}

template<class Node>
void L::BinaryTree<Node>::insert(NodeSide where, BinaryTree<Node> &to_be_inserted) {
    if (has_child(where)) {
        if (to_be_inserted.has_child(where)) {
            throw Exception("You are trying to attach node (n2) to the right of (n1). "
                            "Because (n1) already has a right-node (n3), (n3) should be moved to the right of (n2). "
                            "However, (n2) already has a right-node.");
        }
        to_be_inserted.attach(where, where == Right ? *_right : *_left);
        detach(where);
    }
    attach(where, to_be_inserted);
}

template<class Node>
void L::BinaryTree<Node>::attach(NodeSide where, BinaryTree<Node> &to_be_attached) {
    if (has_child(where)) throw Exception("Trying to attach a right element to (n1) yet (n1) already has a right element.");
    if (where == Right) _right = &to_be_attached; else _left = &to_be_attached;
}

template<class Node>
void L::BinaryTree<Node>::node(const Node& node) {
    _node = node;
}

template<class Node>
void L::BinaryTree<Node>::detach(NodeSide where) {
    if (!has_child(where)) throw Exception("Cannot detach nothing");
    if(where == Left) _left = nullptr; else _right = nullptr;
}

template<class Node>
void L::BinaryTree<Node>::detach_delete(NodeSide where) {
    if (!has_child(where)) throw Exception("Cannot detach nothing");
    BinaryTree<Node>& child = this->child(where);
    this->detach(where);
    delete &child;
}

template<class Node>
void L::BinaryTree<Node>::reduce_to(NodeSide where) {
    if (!has_child(where)) throw Exception("Cannot reduce to left = nothing");
    _node = child(where)._node;

    NodeSide the_other_side = where == Right ? Left : Right;
    if (child(where).has_child(the_other_side)) {
        if (has_child(the_other_side)) throw Exception("Reducing would cause losing one child");
        attach(the_other_side, child(where).child(the_other_side));
    }

    if (child(where).has_child(where)) {
        BinaryTree<Node>& current = child(where).child(where);
        detach(where);
        attach(where, current);
    }
    else {
        detach(where);
    }
}

template<class Node>
void L::BinaryTree<Node>::reduce_to_delete(NodeSide where) {
    if (!has_child(where)) throw Exception("Cannot reduce to left = nothing");
    BinaryTree<Node>& child = this->child(where);
    reduce_to(where);
    if (child.has_child(where)) child.detach(where);
    // if (child.has_child(Left)) child.detach(Left);
    // if (child.has_child(Right)) child.detach(Right);
    delete &child;
}

template<class Node>
void L::BinaryTree<Node>::depth_first_transform(BinaryTree<Node>::TraversalOrder order, const std::function<bool(BinaryTree<Node>&)> &treat_node) {
    switch (order) {
        case InOrder: return depth_first_transform_in_order(treat_node);
        case PostOrder: return depth_first_transform_post_order(treat_node);
        case PreOrder: return depth_first_transform_pre_order(treat_node);
        default: throw Exception("Unknown traversal order for depth first");
    }
}

template<class Node>
void L::BinaryTree<Node>::depth_first_search(BinaryTree<Node>::TraversalOrder order, const std::function<void(const BinaryTree<Node>&)> &treat_node) const {
    switch (order) {
        case InOrder: return depth_first_search_in_order(treat_node);
        case PostOrder: return depth_first_search_post_order(treat_node);
        case PreOrder: return depth_first_search_pre_order(treat_node);
        default: throw Exception("Unknown traversal order for depth first");
    }
}

template<class Node>
void L::BinaryTree<Node>::depth_first_transform_post_order(const std::function<bool(BinaryTree<Node>&)> &treat_node) {

    std::function<void(BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal](BinaryTree<Node>& current){
        if (current.has_child(Left)) traversal(current.child(Left));
        if (current.has_child(Right)) traversal(current.child(Right));
        if(treat_node(current)) traversal(current);
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::depth_first_transform_in_order(const std::function<bool(BinaryTree<Node> &)> &treat_node) {

    std::function<void(BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal](BinaryTree<Node>& current){
        if (current.has_child(Left)) traversal(current.child(Left));
        treat_node(current);
        if (current.has_child(Right)) traversal(current.child(Right));
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::depth_first_transform_pre_order(const std::function<bool(BinaryTree<Node> &)> &treat_node) {

    std::function<void(BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal](BinaryTree<Node>& current){
        treat_node(current);
        if (current.has_child(Left)) traversal(current.child(Left));
        if (current.has_child(Right)) traversal(current.child(Right));
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::depth_first_search_post_order(const std::function<void(const BinaryTree<Node>&)> &treat_node) const {

    std::function<void(const BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal, this](const BinaryTree<Node>& current){
        if (current.has_child(Left)) traversal(current.child(Left));
        if (current.has_child(Right)) traversal(current.child(Right));
        treat_node(current);
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::depth_first_search_in_order(const std::function<void(const BinaryTree<Node> &)> &treat_node) const {

    std::function<void(const BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal](const BinaryTree<Node>& current){
        if (current.has_child(Left)) traversal(current.child(Left));
        treat_node(current);
        if (current.has_child(Right)) traversal(current.child(Right));
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::depth_first_search_pre_order(const std::function<void(const BinaryTree<Node> &)> &treat_node) const {

    std::function<void(const BinaryTree<Node>&)> traversal;
    traversal = [&treat_node, &traversal](const BinaryTree<Node>& current){
        treat_node(current);
        if (current.has_child(Left)) traversal(current.child(Left));
        if (current.has_child(Right)) traversal(current.child(Right));
    };

    traversal(*this);
}

template<class Node>
void L::BinaryTree<Node>::breadth_first_transform(const std::function<void(BinaryTree<Node> &)> &treat_node) {
    std::stack<BinaryTree<Node>*> stack;
    stack.push(this);
    while (!stack.empty()) {
        BinaryTree<Node>* current = stack.top();
        stack.pop();

        treat_node(*current);
        if (current->has_child(Left)) stack.push(&current->child(Left));
        if (current->has_child(Right)) stack.push(&current->child(Right));
    }
}

template<class Node>
void L::BinaryTree<Node>::breadth_first_search(const std::function<bool(const BinaryTree<Node> &)> &treat_node) const {
    std::stack<BinaryTree<Node> const*> stack;
    stack.push(this);
    while (!stack.empty()) {
        BinaryTree<Node> const* current = stack.top();
        stack.pop();

        if (treat_node(*current)) break;
        if (current->has_child(Left)) stack.push(&current->child(Left));
        if (current->has_child(Right)) stack.push(&current->child(Right));
    }
}

template<class Node>
std::string L::BinaryTree<Node>::to_string() const {
    return std::to_string(id());
}

template<class Node>
void L::BinaryTree<Node>::export_to_dot(const std::string& filename, bool with_system_command) const {
    std::string dot_filename = filename + ".dot";
    std::string png_filename = filename + ".png";

    std::ofstream f;
    f.open(dot_filename);

    f << "graph " + filename << " {\n";
    depth_first_search(PostOrder, [&f](const BinaryTree<Node> &current) {
        f << "\t" << current.id() << "[label=\"" << current.node().to_string() << "\", color=\"" << (current.node().color() ? "blue" : "black") << "\"]\n";
        if (current.has_child(Left)) f << "\t " << current.id() << " -- " << current.child(Left).id() << "[color=\"red\"]\n";
        if (current.has_child(Right)) f << "\t " << current.id() << " -- " << current.child(Right).id() << "\n";
    });
    f << "}";

    f.close();

    if (with_system_command) system(("dot -Tpng " + dot_filename + " > " + png_filename).c_str()); // TODO should one delete c_star pointer ? Oo
}

template<class Node>
Node &L::BinaryTree<Node>::node() {
    return _node;
}

template <class Node>
void L::BinaryTree<Node>::insert_copy(NodeSide where, const BinaryTree<Node>& to_be_inserted) {
    auto& copy = *new BinaryTree<Node>(to_be_inserted);
    insert(where, copy);
}

template <class Node>
void L::BinaryTree<Node>::attach_copy(NodeSide where, const BinaryTree<Node>& to_be_attached) {
    auto& copy = *new BinaryTree<Node>(to_be_attached);
    attach(where, copy);
}

template<class Node>
const Node &L::BinaryTree<Node>::node() const {
    return _node;
}

template<class Node>
std::ostream& L::operator<<(std::ostream& os, const BinaryTree<Node>& node) {
    return (os << node.node().to_string());
}

template <class Node>
void L::BinaryTree<Node>::transfer(NodeSide from_side, BinaryTree<Node>& from_src, NodeSide to_side, BinaryTree<Node>& to_src) {
    if (!from_src.has_child(from_side)) throw Exception("Cannot transfer from non-existing source");
    if (to_src.has_child(to_side)) throw Exception("Cannot transfer to already existing destination");
    auto& moved_child = from_src.child(from_side);
    from_src.detach(from_side);
    to_src.attach(to_side, moved_child);
}

template <class Node>
void L::BinaryTree<Node>::swap(NodeSide side1, BinaryTree<Node>& src1, NodeSide side2, BinaryTree<Node>& src2) {
    if (!src1.has_child(side1)) throw Exception("Cannot swap non-existing source");
    if (!src2.has_child(side2)) throw Exception("Cannot swap non-existing source");
    auto& moved_child = src1.child(side1);
    src1.detach(side1);
    src1.attach(side1, src2.child(side2));
    src2.detach(side2);
    src2.attach(side2, moved_child);
}
