#include "MathcadGenerator.hpp"
namespace mcon
{
    MathcadGenerator::MathcadGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    MathcadGenerator::~MathcadGenerator()
    { }

    const std::unordered_map<NodeType, String>& MathcadGenerator::GetMathTemplates()
    {
        return math_templates;
    }

    const std::unordered_map<String, String>& MathcadGenerator::GetSubstitutionList()
    {
        return substitution_list;
    }

    String MathcadGenerator::GenerateMatrix(std::shared_ptr<Node> a_node)
    {
        String matrix_begin = STR("(@MATRIX");
        String matrix_end = STR(")");
        
        String result = matrix_begin;

        for (uint64_t index = 0; index < a_node->child_node_count; ++index)
        {
            result += STR(" ") + ApplyTemplates(a_node->child_nodes.at(index));
        }

        result += matrix_end;

        return result;
    }

    String MathcadGenerator::GenerateCompositeText(std::shared_ptr<Node> a_node)
    {
        uint64_t item_count = a_node->child_node_count;
        String text_start = STR("(@ID");
        String text_end = STR(")");

        String result = text_start;

        for (uint64_t item = 0; item < item_count; ++item)
        {
            result += STR(" ") + ApplyTemplates(a_node->child_nodes.at(item));
        }

        result += text_end;

        return result;
    }
}