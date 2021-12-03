#ifndef __LEXER_H__
#define __LEXER_H__

// Standard libraries
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <stdexcept>

// Custom headers
#include "CharacterStream.hpp"
#include "CharacterSet.hpp"
#include "Token.hpp"

namespace mcon
{
    class Lexer
    {
        public:
            Lexer(  
                std::unique_ptr<CharacterStream> const &a_character_stream,
                std::unique_ptr<CharacterSet> const &a_character_set
            );
            ~Lexer();

            void Scan();
            Token Peek(uint8_t a_offset);
            Token Consume(uint8_t a_offset);
            
            std::vector<Token> tokens;
        
        private:
            std::unique_ptr<CharacterStream> const &character_stream;
            std::unique_ptr<CharacterSet> const &character_set;
            
    };
}

#endif // __LEXER_H__