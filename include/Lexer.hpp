#ifndef __LEXER_H__
#define __LEXER_H__

// Standard libraries
#include <string>
#include <vector>
#include <set>

// Custom headers
#include "CharacterStream.hpp"

namespace mcon
{
    struct CharacterSets
    {
        std::set<char> end_of_stream = {'\0'};
        std::set<char> whitespace = {' ', '\t', '\v', '\n', '\r'};
        std::set<char> letter = {   'a', 'b', 'c', 'd', 'e', 'f', 'g',
                                    'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                    'o', 'p', 'q', 'r', 's', 't', 'u',
                                    'v', 'w', 'x', 'y', 'z'};
    };

    class Lexer
    {
        public:
            Lexer(CharacterStream a_characters);
            ~Lexer();
        
        private:
            std::vector<std::string> tokens;
            
    };
}

#endif // __LEXER_H__