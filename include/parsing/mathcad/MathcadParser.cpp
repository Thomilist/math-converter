#include "MathcadParser.hpp"



namespace mcon
{
    MathcadParser::MathcadParser(std::unique_ptr<Lexer> a_lexer)
        : Parser(std::move(a_lexer))
    { }
    
    MathcadParser::~MathcadParser()
    { }
    
    void MathcadParser::Parse(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        depth = 0;
        Token current_token(TokenType::StartOfStream);

        while (current_token.type == TokenType::StartOfStream)
        {
            current_token = lexer->Consume(0);
        }
        
        do
        {
            switch (state)
            {
                case ParserState::LookingForExpression:
                {
                    if (current_token.content == L"(")
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        current_node->AddChildNode();
                        a_parsing_tree->SetCurrentNode(current_node->child_nodes.back());
                        state = ParserState::IdentifyingOperator;
                        depth++;
                    }
                    else if (current_token.content == L")")
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        a_parsing_tree->SetCurrentNode(current_node->parent_node.lock());
                        depth--;
                    }
                    else if (current_token.type == TokenType::Number)
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        current_node->AddChildNode();
                        auto child_node = current_node->child_nodes.back();
                        child_node->type = NodeType::Number;
                        child_node->content = current_token.content;
                    }
                    else if (current_token.type == TokenType::Text || current_token.type == TokenType::Symbol)
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        current_node->AddChildNode();
                        auto child_node = current_node->child_nodes.back();
                        child_node->type = NodeType::Text;
                        child_node->content = current_token.content;

                        while ((lexer->Peek(0).type == TokenType::Text      ||
                                lexer->Peek(0).type == TokenType::Symbol    ||
                                lexer->Peek(0).type == TokenType::Number)   &&
                                lexer->Peek(0).content != L")")
                        {
                            child_node->content = child_node->content + lexer->Consume(0).content;
                        }
                    }

                    break;
                }
                case ParserState::IdentifyingOperator:
                {
                    std::wstring current_math_operator;
                    
                    if (lexer->Peek(0).type != TokenType::Whitespace)
                    {
                        current_math_operator = current_token.content + lexer->Consume(0).content;
                    }
                    else
                    {
                        current_math_operator = current_token.content;
                    }

                    auto current_node = a_parsing_tree->current_node.lock();

                    try
                    {
                        current_node->type = math_operators.at(current_math_operator);
                    }
                    catch(const std::out_of_range& e)
                    {
                        std::wcerr << L"Out-of-range exception in " << e.what() << L"\n";
                        std::wcerr << L"Unknown math operator: " << current_math_operator << L"\n";
                        return;
                    }

                    state = ParserState::LookingForExpression;

                    break;
                }
            }

            current_token = lexer->Consume(0);

        } while (depth && current_token.type != TokenType::EndOfStream);

        return;
    }
    
    void MathcadParser::Clean(std::shared_ptr<Node> a_node)
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

        // Remove @PLACEHOLDER and @RPLACEHOLDER that Mathcad uses in blank fields
        if (    a_node->type != NodeType::Label &&
                a_node->child_node_count > 0
        )
        {
            for (auto child_node : a_node->child_nodes)
            {
                if (    child_node->type == NodeType::Text  &&
                        (child_node->content == L"@PLACEHOLDER" || child_node->content == L"@RPLACEHOLDER")
                )
                {
                    child_node->content = L"";
                }
            }
        }
    }
}