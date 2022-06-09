#ifndef __LEXER_H__
#define __LEXER_H__

#include "definitions.hpp"

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
                std::shared_ptr<CharacterSet> a_character_set
            );
            ~Lexer();

            void Scan();
            void Reset();
            Token Peek(uint8_t a_offset);
            Token Consume(uint8_t a_offset);

            std::unique_ptr<CharacterStream> character_stream;
            std::shared_ptr<CharacterSet> character_set;
        
        private:
            std::vector<Token> tokens;
            uint64_t index = 0;
            
    };
}

#endif // __LEXER_H__