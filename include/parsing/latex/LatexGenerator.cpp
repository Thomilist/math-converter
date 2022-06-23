#include "LatexGenerator.hpp"



namespace mcon
{
    LatexGenerator::LatexGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    LatexGenerator::~LatexGenerator()
    { }
    
    void LatexGenerator::Generate(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        a_parsing_tree->output = STR("");
        
        // Only allow generation if the parsing tree is not empty
        if (a_parsing_tree->root_node->child_node_count > 0)
        {
            a_parsing_tree->output = ApplyTemplates(a_parsing_tree->root_node->child_nodes.at(0));
        }

        return;
    }
    
    String LatexGenerator::ApplyTemplates(std::shared_ptr<Node> a_node)
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

        // Iterate over tokens to find LaTeX expression placeholders
        while (current_token.type != TokenType::EndOfStream)
        {
            // LaTeX expressions placeholders begin with #...
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
                        ERROR_OUTPUT << STR("LaTeX template indexing error.\n");
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
            else
            {
                if (a_node->child_node_count > 0)
                {
                    result += current_token.content;
                }
                else
                {
                    if (a_node->type == NodeType::Number)
                    {
                        a_node->content = FormatComplexNumber(a_node->content);
                    }
                    
                    result += a_node->content;
                }

                current_token = lexer.Consume(0);
            }
        }

        return result;
    }
    
    void LatexGenerator::Substitute(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        for (auto& substitution_item : substitution_list)
        {
            while (true)
            {
                std::size_t position = a_parsing_tree->output.find(substitution_item.first);

                if (position == String::npos)
                {
                    break;
                }

                a_parsing_tree->output.replace(position, substitution_item.first.length(), substitution_item.second);
            }
        }
        return;
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
    
    String LatexGenerator::FormatComplexNumber(String a_number)
    {
        int index = a_number.length() - 1;

        if (a_number.at(index) == STR('i'))
        {
            a_number.replace(index, 1, STR("\\mathrm{i}"));
        }
        else if (a_number.at(index) == STR('j'))
        {
            a_number.replace(index, 1, STR("\\mathrm{j}"));
        }

        return a_number;
    }
}