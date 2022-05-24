#ifndef __PARSER_H__
#define __PARSER_H__

// Standard libraries
#include <string>
#include <memory>

// Custom headers
#include "../Lexer.hpp"
#include "intermediate/Node.hpp"

namespace mcon
{
    class Parser
    {
        public:
            Parser(std::unique_ptr<Lexer> a_lexer);
            ~Parser();

            std::unique_ptr<Node> parse();

            std::unique_ptr<Lexer> lexer;

        private:
            
    };
}

#endif // __PARSER_H__