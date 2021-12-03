#include "Token.hpp"


namespace mcon
{
    Token::Token(TokenType a_type):
        type(a_type)
    { }

    Token::~Token() 
    { }

    bool Token::append(std::string a_character)
    {
        content.append(a_character);
        return true;
    }
}