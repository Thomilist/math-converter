#ifndef __TOKEN_H__
#define __TOKEN_H__

// Standard libraries
#include <string>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

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