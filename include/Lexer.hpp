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
                std::unique_ptr<CharacterStream> a_character_stream,
                std::unique_ptr<CharacterSet> a_character_set
            );
            ~Lexer();

            void Scan();
            Token Peek(uint8_t a_offset);
            Token Consume(uint8_t a_offset);
        
        private:
            std::unique_ptr<CharacterStream> character_stream;
            std::unique_ptr<CharacterSet> character_set;
            
            std::vector<Token> tokens;
            uint64_t index = 0;
            
    };
}

#endif // __LEXER_H__