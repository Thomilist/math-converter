#ifndef __TOKEN_H__
#define __TOKEN_H__

// Standard libraries
#include <string>

namespace mcon
{
    enum class TokenType
    {
        StartOfStream,
        EndOfStream,
        OutOfBounds,
        Whitespace,
        Text,
        Number,
        Symbol
    };

    class Token
    {
        public:
            Token(TokenType a_type);
            ~Token();
            
            bool Append(std::string a_character);

            TokenType type;
            std::string content;
        
        private:
    };
}

#endif // __TOKEN_H__