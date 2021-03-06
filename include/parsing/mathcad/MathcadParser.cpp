#include "MathcadParser.hpp"
#include "../intermediate/ParsingTree.hpp"



namespace mcon
{
    MathcadParser::MathcadParser(std::shared_ptr<Lexer> a_lexer)
        : Parser(a_lexer)
    { }
    
    MathcadParser::~MathcadParser()
    { }
    
    void MathcadParser::Parse(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        // Initialise from start of token stream
        current_token = Token(TokenType::StartOfStream);

        while (current_token.type == TokenType::StartOfStream)
        {
            current_token = lexer->Consume(0);
        }

        state = ParserState::LookingForExpression;
        
        // Parse token stream
        ParseExpression(a_parsing_tree);

        return;
    }
    
    void MathcadParser::ParseExpression(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        String current_math_operator;
        
        do
        {
            switch (state)
            {
                case ParserState::LookingForExpression:
                {
                    // Complex expressions begin with an opening parens
                    // They contain other expressions, so this increases the parsing depth
                    if (current_token.content == STR("("))
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        current_node->AddChildNode();
                        a_parsing_tree->SetCurrentNode(current_node->child_nodes.back());
                        
                        // An opening parens is always followed by an operator
                        state = ParserState::IdentifyingOperator;

                        current_token = lexer->Consume(0);

                        ParseExpression(a_parsing_tree);
                    }
                    // Complex expressions end with a closing parens
                    // This decreases the parsing depth
                    else if (current_token.content == STR(")"))
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        a_parsing_tree->SetCurrentNode(current_node->parent_node.lock());
                        return;
                    }
                    // Numbers do not contain other expressions, so the parsing depth is unaffected
                    else if (current_token.type == TokenType::Number)
                    {
                        auto current_node = a_parsing_tree->current_node.lock();
                        current_node->AddChildNode();
                        auto child_node = current_node->child_nodes.back();
                        child_node->type = NodeType::Number;
                        child_node->content = current_token.content;

                        // Handling of decimal numbers
                        if (    lexer->Peek(0).type == TokenType::Symbol    &&
                                lexer->Peek(0).content == STR(".")          &&
                                lexer->Peek(1).type == TokenType::Number    )
                        {
                            for (int i = 0; i < 2; i++)
                            {
                                current_token = lexer->Consume(0);
                                child_node->content += current_token.content;
                            }
                        }

                        // Handling of complex numbers
                        if (    lexer->Peek(0).type == TokenType::Text                                      &&
                                (lexer->Peek(0).content == STR("i") || lexer->Peek(0).content == STR("j"))  )
                        {
                            current_token = lexer->Consume(0);
                            child_node->content += current_token.content;
                        }
                    }
                    // Symbols and basic text do not contain other expressions, so the parsing depth is unaffected
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
                                lexer->Peek(0).content != STR(")")          )
                        {
                            child_node->content = child_node->content + lexer->Consume(0).content;
                        }

                        // Handle Mathcad placeholders
                        if (    child_node->content == STR("@PLACEHOLDER")  ||
                                child_node->content == STR("@RPLACEHOLDER") )
                        {
                            child_node->type = NodeType::Void;
                        }
                    }

                    break;
                }
                case ParserState::IdentifyingOperator:
                {
                    // Math operators consist of at least one symbol
                    current_math_operator = current_token.content;

                    // Some math operators consists of two symbols, or of an @ and a word
                    while (lexer->Peek(0).type != TokenType::Whitespace)
                    {
                        current_math_operator += lexer->Consume(0).content;
                    }

                    auto current_node = a_parsing_tree->current_node.lock();

                    // Conversion can only happen for operators defined in the math_operators map
                    try
                    {
                        current_node->type = math_operators.at(current_math_operator);
                    }
                    catch(const std::out_of_range& e)
                    {
                        ERROR_OUTPUT << STR("Unknown math operator: ") << current_math_operator << STR("\n");
                        ERROR_OUTPUT << STR("Out-of-range exception in ") << e.what() << STR("\n") << std::endl;
                        
                        state = ParserState::LookingForExpression;
                        return;
                    }

                    // Operators are always followed by at least one expression
                    state = ParserState::LookingForExpression;

                    break;
                }
            }

            current_token = lexer->Consume(0);
        } while (   current_token.type != TokenType::EndOfStream    &&
                    current_token.type != TokenType::OutOfBounds    );

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
                if (    child_node->type == NodeType::Text                                                          &&
                        (child_node->content == STR("@PLACEHOLDER") || child_node->content == STR("@RPLACEHOLDER")) )
                {
                    child_node->content = STR("");
                }
            }
        }

        // Escape characters
        for (auto& escape_character : escape_list)
        {
            EscapeCharacter(a_node, escape_character.first, escape_character.second);
        }

        // Correct type of matrix size nodes
        if (a_node->type == NodeType::Matrix)
        {
            a_node->child_nodes.at(0)->type = NodeType::MatrixRowCount;
            a_node->child_nodes.at(1)->type = NodeType::MatrixCollumnCount;
        }

        return;
    }
    
    void MathcadParser::EscapeCharacter(std::shared_ptr<Node> a_node, String a_find, String a_replace)
    {
        if (a_node->type == NodeType::Text)
        {
            std::size_t position = a_node->content.find(a_find);

            if (position != String::npos)
            {
                a_node->content.replace(position, a_find.length(), a_replace);
            }
        }
    }
}