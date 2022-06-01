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
        a_parsing_tree->output = Substitute(a_parsing_tree->root_node->child_nodes.front());
        return;
    }
    
    std::string LatexGenerator::Substitute(std::shared_ptr<Node> a_node)
    {
        std::string result = "";
        
        try
        {
            std::string template_text = math_operators.at(a_node->type);
            auto template_stream = std::make_unique<CharacterStream>(template_text);
            auto new_character_set = std::make_shared<mcon::CharacterSet>();
            new_character_set->LoadFromFolder(".\\resources\\character-sets");
            auto lexer = Lexer(std::move(template_stream), new_character_set);
            lexer.Scan();

            Token current_token(TokenType::StartOfStream);

            while (current_token.type == TokenType::StartOfStream)
            {
                current_token = lexer.Consume(0);
            }

            while (current_token.type != TokenType::EndOfStream)
            {
                if (current_token.content == "#")
                {
                    current_token = lexer.Consume(0);

                    if (current_token.type == TokenType::Number)
                    {
                        int index = std::stoi(current_token.content);
                        result += Substitute(a_node->child_nodes.at(index));
                        current_token = lexer.Consume(0);
                    }
                    else
                    {
                        result += "MATH_TEMPLATE_ERROR";
                    }
                }
                else
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
        }
        catch(const std::out_of_range& e)
        {
            std::cerr << "Out-of-range exception in " << e.what() << "\n";
            std::cerr << "Unable to print math expression.\n";
            return "MATH_OPERATOR_ERROR";
        }

        return result;
    }
}