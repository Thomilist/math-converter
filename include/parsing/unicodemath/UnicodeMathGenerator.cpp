#include "UnicodeMathGenerator.hpp"

namespace mcon
{
    UnicodeMathGenerator::UnicodeMathGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    UnicodeMathGenerator::~UnicodeMathGenerator()
    { }

    const std::unordered_map<NodeType, String>& UnicodeMathGenerator::GetMathTemplates()
    {
        return math_templates;
    }

    const std::unordered_map<String, String>& UnicodeMathGenerator::GetSubstitutionList()
    {
        return substitution_list;
    }
    
    String UnicodeMathGenerator::GenerateMatrix(std::shared_ptr<Node> a_node)
    {
        uint64_t row_count = std::stoi(a_node->child_nodes.at(0)->content);
        uint64_t collumn_count = std::stoi(a_node->child_nodes.at(1)->content);
        
        String matrix_begin = STR("[\\matrix (");
        String matrix_end = STR(")]");
        String matrix_break = STR("@");
        String matrix_delimiter = STR("&");

        String result = matrix_begin;

        for (uint64_t row = 0; row < row_count; ++row)
        {
            for (uint64_t collumn = 0; collumn < collumn_count; ++collumn)
            {
                uint64_t index = 2 + row * collumn_count + collumn;
                result += ApplyTemplates(a_node->child_nodes.at(index));

                if (collumn + 1 < collumn_count)
                {
                    result += matrix_delimiter;
                }
            }

            if ((row + 1) < row_count)
            {
                result += matrix_break;
            }
        }

        result += matrix_end;

        return result;
    }

    String UnicodeMathGenerator::GenerateCompositeText(std::shared_ptr<Node> a_node)
    {
        uint64_t item_count = a_node->child_node_count;
        String result = STR("");

        for (uint64_t item = 1; item < item_count; ++item)
        {
            if (a_node->child_nodes.at(item)->type == NodeType::TextSubscript)
            {
                a_node->child_nodes.at(item)->AddChildNode();
                a_node->child_nodes.at(item)->child_nodes.back() = a_node->child_nodes.at(item)->child_nodes.front();
                a_node->child_nodes.at(item)->child_nodes.front() = std::make_shared<Node>(*a_node->child_nodes.at(item - 1));

                a_node->child_nodes.at(item - 1)->content = STR("");
            }
        }

        for (uint64_t item = 0; item < item_count; ++item)
        {
            result += ApplyTemplates(a_node->child_nodes.at(item));
        }

        return result;
    }
}