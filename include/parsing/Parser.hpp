#ifndef __PARSER_H__
#define __PARSER_H__

// Standard libraries
#include <string>
#include <memory>

// Custom headers
#include "../Lexer.hpp"
#include "intermediate/Node.hpp"
#include "intermediate/ParsingTree.hpp"

namespace mcon
{
    // Forward declaration to work around circular dependency
    class ParsingTree;
    
    class Parser
    {
        public:
            Parser(std::unique_ptr<Lexer> a_lexer);
            ~Parser();

            virtual void Parse(std::shared_ptr<ParsingTree> a_parsing_tree);

            std::unique_ptr<Lexer> lexer;

        private:
            
    };
}

#endif // __PARSER_H__