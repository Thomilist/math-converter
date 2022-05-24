#include "MathcadParser.hpp"



namespace mcon
{
    MathcadParser::MathcadParser(std::unique_ptr<Lexer> a_lexer)
        : Parser(std::move(a_lexer))
    { }
    
    MathcadParser::~MathcadParser()
    { }
}