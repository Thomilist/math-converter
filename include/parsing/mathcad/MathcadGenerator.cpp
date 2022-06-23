#include "MathcadGenerator.hpp"
namespace mcon
{
    MathcadGenerator::MathcadGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    MathcadGenerator::~MathcadGenerator()
    { }
    
    void MathcadGenerator::Generate(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        a_parsing_tree->output = STR("");
        
        // Only allow generation if the parsing tree is not empty
        if (a_parsing_tree->root_node->child_node_count > 0)
        {
            a_parsing_tree->output = ApplyTemplates(a_parsing_tree->root_node->child_nodes.at(0));
        }

        return;
    }
    
    String MathcadGenerator::ApplyTemplates(std::shared_ptr<Node> a_node)
    {
        // Special case for matrices
        if (a_node->type == NodeType::Matrix)
        {
            return GenerateMatrix(a_node);
        }
        
        String result = STR("");
        String template_text;
        
        // Fetch math operator template text
        try
        {
            template_text = math_operators.at(a_node->type);
        }
        catch(const std::out_of_range& e)
        {
            ERROR_OUTPUT << STR("Unable to print math expression.\n");
            ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
            return STR("#ERROR");
        }

        // Run the template text through a lexer
        auto template_stream = std::make_unique<CharacterStream>(template_text);
        auto lexer = Lexer(std::move(template_stream), character_set);
        lexer.Scan();

        // Prepare token stream
        Token current_token(TokenType::StartOfStream);

        while (current_token.type == TokenType::StartOfStream)
        {
            current_token = lexer.Consume(0);
        }

        // Iterate over tokens to find Mathcad expression placeholders
        while (current_token.type != TokenType::EndOfStream)
        {
            // Mathcad expression placeholders begin with #...
            if (    current_token.content == STR("#")   &&
                    lexer.Peek(-1).content != STR("\\")
            )
            {
                current_token = lexer.Consume(0);
                
                // ... and the # is followed by a number indicating the child node index to fetch content from
                if (current_token.type == TokenType::Number)
                {
                    int index = std::stoi(current_token.content);

                    try
                    {
                        result += ApplyTemplates(a_node->child_nodes.at(index));
                    }
                    catch(const std::out_of_range& e)
                    {
                        ERROR_OUTPUT << STR("Mathcad template indexing error.\n");
                        ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
                        result += STR("#ERROR");
                    }
                    
                    current_token = lexer.Consume(0);
                }
                else
                {
                    result += STR("#ERROR");
                }
            }
            // Non-placeholder text is simply appended
            {
                if (a_node->child_node_count > 0)
                {
                    result += current_token.content;
                }
                else
                {
                    result += a_node->content;
                }

                current_token = lexer.Consume(0);
            }
        }

        return result;
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
}