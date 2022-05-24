#include "Node.hpp"


namespace mcon
{
    Node::Node()
    { }

    Node::~Node()
    { }

    void Node::add_child_node()
    {
        child_nodes.push_back(std::move(std::shared_ptr<Node>(new Node())));
        child_nodes.back()->parent_node = shared_from_this();
        return;
    }

    void Node::delete_child_node(uint64_t a_index)
    {
        child_nodes.at(a_index).reset();
        child_nodes.erase(child_nodes.begin() + (a_index - 1));
        return;
    }
}