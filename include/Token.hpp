#ifndef __TOKEN_H__
#define __TOKEN_H__

// Standard libraries
#include <string>

namespace mcon
{
    enum class TokenType
    {
        EndOfStream,
        Whitespace,
        Text,
        Number,
        Symbol
    };

    class Token
    {
        public:
            Token();
            ~Token();
        
        private:
    };
}

#endif // __TOKEN_H__