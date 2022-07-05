#include "LatexGenerator.hpp"



namespace mcon
{
    LatexGenerator::LatexGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    LatexGenerator::~LatexGenerator()
    { }

    const std::unordered_map<NodeType, String>& LatexGenerator::GetMathTemplates()
    {
        return math_templates;
    }

    const std::unordered_map<String, String>& LatexGenerator::GetSubstitutionList()
    {
        return substitution_list;
    }
    
    String LatexGenerator::GenerateMatrix(std::shared_ptr<Node> a_node)
    {
        uint64_t row_count = std::stoi(a_node->child_nodes.at(0)->content);
        uint64_t collumn_count = std::stoi(a_node->child_nodes.at(1)->content);
        
        String matrix_begin = STR("\\left[\\begin{matrix}");
        String matrix_end = STR("\\end{matrix}\\right]");
        String matrix_break = STR("\\\\[0.0em]");
        
        String result = matrix_begin;

        for (uint64_t row = 0; row < row_count; ++row)
        {
            for (uint64_t collumn = 0; collumn < collumn_count; ++collumn)
            {
                uint64_t index = 2 + row * collumn_count + collumn;
                result += STR("{") + ApplyTemplates(a_node->child_nodes.at(index)) + STR("}");

                if ((collumn + 1) < collumn_count)
                {
                    result += STR("&");
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

    String LatexGenerator::GenerateCompositeText(std::shared_ptr<Node> a_node)
    {
        uint64_t item_count = a_node->child_node_count;
        String result = STR("");

        for (uint64_t item = 0; item < item_count; ++item)
        {
            result += ApplyTemplates(a_node->child_nodes.at(item));
        }

        return result;
    }
}