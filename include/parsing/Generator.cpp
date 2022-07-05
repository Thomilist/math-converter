#include "Generator.hpp"



namespace mcon
{
    Generator::Generator(std::shared_ptr<CharacterSet> a_character_set)
        : character_set(a_character_set)
    { }
    
    Generator::~Generator()
    { }
    
    
    void Generator::Generate(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        a_parsing_tree->output = STR("");
        
        // Only allow generation if the parsing tree is not empty
        if (a_parsing_tree->root_node->child_node_count > 0)
        {
            a_parsing_tree->output = ApplyTemplates(a_parsing_tree->root_node->child_nodes.at(0));
        }

        Substitute(a_parsing_tree);

        return;
    }

    void Generator::Substitute(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        for (auto& substitution_item : GetSubstitutionList())
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
    
    String Generator::ApplyTemplates(std::shared_ptr<Node> a_node)
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
        
        Lexer lexer = LoadMathTemplate(a_node);
        String result = STR("");

        while (lexer.Consume().type != TokenType::EndOfStream)
        {
            if (FoundMathPlaceholder(lexer))
            {
                result += ProcessMathPlaceholder(lexer, a_node);
            }
            else
            {
                result += ProcessNonPlaceholder(lexer, a_node);
            }
        }

        return result;
    }
    
    Lexer Generator::LoadMathTemplate(std::shared_ptr<Node> a_node)
    {
        String template_text;

        // Fetch math operator template text
        try
        {
            template_text = GetMathTemplates().at(a_node->type);
        }
        catch(const std::out_of_range& e)
        {
            ERROR_OUTPUT << STR("Unable to print math expression.\n");
            ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
            template_text = STR("#ERROR");
        }
        
        // Run the template text through a lexer
        auto template_stream = std::make_unique<CharacterStream>(template_text);
        return Lexer(std::move(template_stream), character_set);
    }
    
    bool Generator::FoundMathPlaceholder(Lexer& a_lexer)
    {
        return a_lexer.Peek(0).content == STR("#") && a_lexer.Peek(-1).content != STR("\\");
    }
    
    String Generator::ProcessMathPlaceholder(Lexer& a_lexer, std::shared_ptr<Node> a_node)
    {
        a_lexer.Consume();
        
        if (a_lexer.Peek().type == TokenType::Number)
        {
            if (a_node->child_node_count > 0)
            {
                try
                {
                    int index = std::stoi(a_lexer.Peek().content);
                    return ApplyTemplates(a_node->child_nodes.at(index));
                }
                catch(const std::out_of_range& e)
                {
                    ERROR_OUTPUT << STR("Math template indexing error.\n");
                    ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
                    return STR("#ERROR");
                }
            }
            else
            {
                return a_node->content;
            }

            if (a_lexer.Peek().type != TokenType::EndOfStream)
            {
                a_lexer.Consume();
            }
        }
        else
        {
            return STR("#ERROR"); 
        }
    }
    
    String Generator::ProcessNonPlaceholder(Lexer& a_lexer, std::shared_ptr<Node> a_node)
    {
        if (a_lexer.character_stream->buffer == STR(" "))
        {
            a_lexer.Consume();
            return a_node->content;
        }
        else
        {
            // Avoid returning a string containing the null character,
            // instead returning an empty string
            if (a_lexer.Peek().type == TokenType::EndOfStream)
            {
                return String{};
            }
            else
            {
                return a_lexer.Peek().content;
            }
        }
    }

    // Virtual function intentionally have incomplete implementations
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    String Generator::GenerateMatrix(std::shared_ptr<Node> a_node)
    {
        return STR("#MATRIX");
    }
    
    String Generator::GenerateCompositeText(std::shared_ptr<Node> a_node)
    {
        return STR("#COMPOSITETEXT");
    }
    #pragma GCC diagnostic pop
    
    const std::unordered_map<NodeType, String>& Generator::GetMathTemplates()
    {
        return math_templates;
    }

    
    const std::unordered_map<String, String>& Generator::GetSubstitutionList()
    {
        return substitution_list;
    }
}