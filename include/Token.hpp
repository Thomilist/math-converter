#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "definitions.hpp"

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
        Symbol,
        Unknown
    };

    class Token
    {
        public:
            Token(TokenType a_type);
            ~Token();
            
            bool Append(String a_character);

            TokenType type;
            String content;
        
        private:
    };
}

#endif // __TOKEN_H__