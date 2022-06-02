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
        a_parsing_tree->output = L"";
        
        // Only allow generation if the parsing tree is not empty
        if (a_parsing_tree->root_node->child_node_count > 0)
        {
            a_parsing_tree->output = ApplyTemplates(a_parsing_tree->root_node->child_nodes.at(0));
        }
        return;
    }
    
    std::wstring LatexGenerator::ApplyTemplates(std::shared_ptr<Node> a_node)
    {
        std::wstring result = L"";
        std::wstring template_text;
        
        // Fetch math operator template text
        try
        {
            template_text = math_operators.at(a_node->type);
        }
        catch(const std::out_of_range& e)
        {
            std::wcerr << L"Unable to print math expression.\n";
            std::wcerr << L"Out-of-range exception in " << e.what() << "\n";
            return L"#ERROR";
        }

        // Run the template text through a lexer
        auto template_stream = std::make_unique<CharacterStream>(template_text);
        auto new_character_set = std::make_shared<mcon::CharacterSet>();
        new_character_set->LoadFromFolder(".\\resources\\character-sets");
        auto lexer = Lexer(std::move(template_stream), new_character_set);
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
            if (current_token.content == L"#")
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
                        std::wcerr << L"LaTeX template indexing error.\n";
                        std::wcerr << L"Out-of-range exception in " << e.what() << "\n";
                        result += L"#ERROR";
                    }

                    current_token = lexer.Consume(0);
                }
                else
                {
                    result += L"#ERROR";
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

                if (position == std::wstring::npos)
                {
                    break;
                }

                a_parsing_tree->output.replace(position, substitution_item.first.length(), substitution_item.second);
            }
        }
        return;
    }
}