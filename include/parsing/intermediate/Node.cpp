#include "Node.hpp"


namespace mcon
{
    Node::Node()
    { }

    Node::~Node()
    { }

    void Node::AddChildNode()
    {
        child_nodes.push_back(std::move(std::make_shared<Node>()));
        child_nodes.back()->parent_node = shared_from_this();
        return;
    }

    void Node::DeleteChildNode(uint64_t a_index)
    {
        child_nodes.at(a_index).reset();
        child_nodes.erase(child_nodes.begin() + (a_index - 1));
        return;
    }
}