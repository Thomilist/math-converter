#include "ParsingTree.hpp"



namespace mcon
{
    ParsingTree::ParsingTree(
        std::unique_ptr<Parser> a_parser, 
        std::unique_ptr<Generator> a_generator
    ):
        parser(std::move(a_parser)),
        generator(std::move(a_generator))  
    {
        root_node = std::move(std::make_shared<Node>());
        root_node->type = NodeType::Root;
        current_node = root_node;
    }
    
    ParsingTree::~ParsingTree()
    { }
    
    void ParsingTree::SetCurrentNode(std::shared_ptr<Node> a_node)
    {
        current_node = std::move(a_node);
        return;
    }
}