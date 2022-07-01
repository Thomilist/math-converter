#include "MathMLGenerator.hpp"

namespace mcon
{
    MathMLGenerator::MathMLGenerator(std::shared_ptr<CharacterSet> a_character_set)
        : Generator(a_character_set)
    { }
    
    MathMLGenerator::~MathMLGenerator()
    { }
    
    void MathMLGenerator::Generate(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        a_parsing_tree->output = STR("");
        
        // Only allow generation if the parsing tree is not empty
        if (a_parsing_tree->root_node->child_node_count > 0)
        {
            a_parsing_tree->output = ApplyTemplates(a_parsing_tree->root_node->child_nodes.at(0));
        }

        return;
    }

    const std::unordered_map<String, String>& MathMLGenerator::GetSubstitutionList()
    {
        return substitution_list;
    }
    
    String MathMLGenerator::ApplyTemplates(std::shared_ptr<Node> a_node)
    {
        // Special cases
        switch (a_node->type)
        {
            case NodeType::Matrix:
            {
                return GenerateMatrix(a_node);
            }
            case NodeType::TextComposite:
            {
                return GenerateCompositeText(a_node);
            }
            default:
            {
                break;
            }
        }
        
        String result = STR("");
        String template_text;
        
        // Fetch math operator template text
        try
        {
            template_text = math_templates.at(a_node->type);
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

        // Iterate over tokens to find MathML expression placeholders
        while (current_token.type != TokenType::EndOfStream)
        {
            // MathML expression placeholders begin with #...
            if (    current_token.content == STR("#")   &&
                    lexer.Peek(-1).content != STR("\\")
            )
            {
                current_token = lexer.Consume(0);
                
                // ... and the # is followed by a number indicating the child node index to fetch content from
                if (current_token.type == TokenType::Number)
                {
                    if (a_node->child_node_count > 0)
                    {
                        int index = std::stoi(current_token.content);

                        try
                        {
                            result += ApplyTemplates(a_node->child_nodes.at(index));
                        }
                        catch(const std::out_of_range& e)
                        {
                            ERROR_OUTPUT << STR("MathML template indexing error.\n");
                            ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
                            result += STR("#ERROR");
                        }
                    }
                    else
                    {
                        result += a_node->content;
                    }
                    
                    current_token = lexer.Consume(0);

                    if (current_token.type == TokenType::EndOfStream)
                    {
                        break;
                    }
                }
                else
                {
                    result += STR("#ERROR");
                }
            }
            // Non-placeholder text is simply appended
            {
                if (template_text != STR(" "))
                {
                    result += current_token.content;
                }
                // When the template text is only a single space, simply append the node contents
                else
                {
                    result += a_node->content;
                }

                current_token = lexer.Consume(0);
            }
        }

        return result;
    }
    
    String MathMLGenerator::GenerateMatrix(std::shared_ptr<Node> a_node)
    {
        uint64_t row_count = std::stoi(a_node->child_nodes.at(0)->content);
        uint64_t collumn_count = std::stoi(a_node->child_nodes.at(1)->content);
        
        String matrix_begin = STR("<mrow><mo>[</mo><mtable><mtr>");
        String matrix_end = STR("</mtr></mtable><mo>]</mo></mrow>");
        String matrix_break = STR("</mtr><mtr>");

        String result = matrix_begin;

        for (uint64_t row = 0; row < row_count; ++row)
        {
            for (uint64_t collumn = 0; collumn < collumn_count; ++collumn)
            {
                uint64_t index = 2 + row * collumn_count + collumn;
                result += STR("<mtd>") + ApplyTemplates(a_node->child_nodes.at(index)) + STR("</mtd>");
            }

            if ((row + 1) < row_count)
            {
                result += matrix_break;
            }
        }

        result += matrix_end;

        return result;
    }

    String MathMLGenerator::GenerateCompositeText(std::shared_ptr<Node> a_node)
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