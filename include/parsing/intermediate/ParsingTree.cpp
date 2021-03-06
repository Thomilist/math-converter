#include "ParsingTree.hpp"



namespace mcon
{
    ParsingTree::ParsingTree(
        std::unordered_map<InputLanguage, std::unique_ptr<Parser>> a_parsers, 
        std::unordered_map<OutputLanguage, std::unique_ptr<Generator>> a_generators
    ):
        parsers(std::move(a_parsers)),
        generators(std::move(a_generators))  
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
    
    void ParsingTree::Reset()
    {
        root_node.reset();
        root_node = std::move(std::make_shared<Node>());
        root_node->type = NodeType::Root;
        current_node = root_node;
        return;
    }
    
    void ParsingTree::Clean(std::shared_ptr<Node> a_node)
    {
        // Recursively iterate over all child nodes
        if (a_node->child_node_count > 0)
        {
            for (auto child_node : a_node->child_nodes)
            {
                Clean(child_node);
            }
        }

        // Actual cleaning happens after this point

        // Substitute the decimal point
        if (a_node->type == NodeType::Number)
        {
            // Look for period as decimal point
            std::size_t position = a_node->content.find(STR("."));
            if (position != String::npos)
            {
                a_node->content.replace(position, 1, decimal_separator);
            }

            // Look for comma as decimal point
            position = a_node->content.find(STR(","));
            if (position != String::npos)
            {
                a_node->content.replace(position, 1, decimal_separator);
            }
        }

        return;
    }
}