#include "Node.hpp"


namespace mcon
{
    Node::Node()
    { }

    Node::~Node()
    { }

    void Node::append_node(std::unique_ptr<Node> a_node)
    {
        child_nodes.push_back(std::move(a_node));
        return;
    }

    void Node::delete_node(uint64_t a_index)
    {
        child_nodes.at(a_index).reset();
        child_nodes.erase(child_nodes.begin() + (a_index - 1));
        return;
    }
}