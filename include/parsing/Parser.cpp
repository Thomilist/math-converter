#include "Parser.hpp"


namespace mcon
{
    Parser::Parser(std::unique_ptr<Lexer> a_lexer)
        : lexer(std::move(a_lexer))
    { }
    
    Parser::~Parser()
    { }
}