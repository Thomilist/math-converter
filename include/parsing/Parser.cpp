#include "Parser.hpp"


namespace mcon
{
    Parser::Parser(std::unique_ptr<Lexer> a_lexer)
        : lexer(std::move(a_lexer))
    { }
    
    Parser::~Parser()
    { }
    
    // Virtual function implementation is intentionally left blank
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    void Parser::Parse(std::shared_ptr<ParsingTree> a_parsing_tree)
    { }
    #pragma GCC diagnostic pop
}