#include "LatexParser.hpp"
#include "../intermediate/ParsingTree.hpp"



namespace mcon
{
    LatexParser::LatexParser(std::unique_ptr<Lexer> a_lexer)
        : Parser(std::move(a_lexer))
    { }
    
    LatexParser::~LatexParser()
    { }
}